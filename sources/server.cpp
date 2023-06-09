#include "irc.hpp"

//__________________________________________________canonic form

Server::Server(const int port, const std::string password)
	: _port(port), _password(password), _flag_keep_loop(true), _flag_notice(false)
{
	if (!setSocketServer())
	{
		FATAL_ERR("cannot create server object because it is not possible to create socket server."
				  << std::endl);
		return;
	}
	setAddrServ();
}

Server::Server(Server const &cpy)
{
	*this = cpy;
}

Server &Server::operator=(Server const &rhs)
{
	if (this != &rhs)
	{
		_port = rhs._port;
		_socket_server = rhs._socket_server;
		_addr_server = rhs._addr_server;
		_client_addr = rhs._client_addr;
		_sin_size = rhs._sin_size;
		_password = rhs._password;
		_flag_keep_loop = rhs._flag_keep_loop;
		_flag_notice = rhs._flag_notice;
		_client = rhs._client;
		_channels = rhs._channels;
	}
	return (*this);
}

Server::~Server(void)
{
	INFO("Destructor server called." << std::endl);

	std::vector<Client *>::iterator it;
	for (it = _client.begin(); it != _client.end(); it++)
	{
		delete (*it);
	}
	_client.clear();
	std::vector<Channel *>::iterator it2;
	for (it2 = _channels.begin(); it2 != _channels.end(); it2++)
	{
		delete (*it2);
	}
	_channels.clear();
	shutdown(this->_socket_server, SHUT_RDWR);
	close(this->_socket_server);
}

//__________________________________________________GETTERS_SETTERS

// sockets_________________________________________________________________

// creer un socket -descirpteur de fichier
// https://man7.org/linux/man-pages/man2/socket.2.html
// AF_INET: IPv4, SOCK_STREAM :TCP, IPPPROTO_TCP :TCP => 0 Indicates
// that the default protocol for the type selected is to be used. For example,
// IPPROTO_TCP is chosen for the protocol if the type was set to SOCK_STREAM
// and the address family is AF_INET.
// return false si la création de la socket a échoué
//__________________________________________________________________________

bool Server::getFlagKeepLoop()
{
	return this->_flag_keep_loop;
}

bool Server::setSocketServer()
{
	this->_socket_server = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket_server == -1)
	{
		perror("socket()");
		return false;
	}
	return true;
}

int Server::getSocketServer()
{
	return this->_socket_server;
}

bool Server::AcceptSocketClient()
{
	setSinSize();

	int socket = accept(_socket_server, (struct sockaddr *)&_client_addr, &_sin_size);
	if (socket == -1)
	{
		perror("accept()");
		return false;
	}
	INFO(socket << std::endl);
	_client.push_back(new Client(socket));

	return true;
}

// data from params_________________________________________________________

int Server::getPort()
{
	return this->_port;
}

std::string Server::getPassword()
{
	return this->_password;
}

// data from sockaddr_______________________________________________________

void Server::setAddrServ()
{
	this->_addr_server.sin_family = AF_INET;		  // host byte order
	this->_addr_server.sin_port = htons(this->_port); // short, network byte order
	this->_addr_server.sin_addr.s_addr = INADDR_ANY;  // a#include <signal.h>uto-remplissage avec  mon IP//inet_addr("127.0.0.1");
	bzero(&(this->_addr_server.sin_zero), 8);		  // interdite?
}

struct sockaddr_in Server::getAddrServ()
{
	return this->_addr_server;
}

void Server::setSinSize()
{
	this->_sin_size = sizeof(struct sockaddr_in);
}

socklen_t Server::getSinSize()
{
	return this->_sin_size;
}

//__________________________________________________MEMBERS FUNCTIONS
// si serveur crash, socket maintenue non reutilisable => autorise  de reutiliser mon adress et port
void Server::crash_protector()
{
	int yes = 1;

	if (setsockopt(_socket_server, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		perror("setsockopt");
		exit(1);
	}
}

// return false en cas d'erreur
// bind= associer la socket avec un port de votre machine locale.
// listen = les connections vont attendre dans la file de BACKLOG jusqu'à ce qu' on
// les accept()
bool Server::startServer()
{
	crash_protector();
	if (bind(_socket_server, (struct sockaddr *)&_addr_server, sizeof(struct sockaddr)) == -1)
	{
		perror("bind");
		return false;
	}
	if (listen(_socket_server, BACKLOG) == -1)
	{
		perror("listen");
		return false;
	}
	return true;
}

//-----loop recept, send , select------------------------------------------------

// https://manpages.ubuntu.com/manpages/xenial/fr/man2/select_tut.2.html
// Afin de gérer ses clients, notre serveur va maintenant devoir maintenir une
// liste de clients connectés en enregistrant les retours de accept qui représente
// chaque client effectivement connecté à notre serveur, et en supprimant ceux
// dont le socket retourne une erreur indiquant qu'ils ont été déconnectés.

// « Gérer ses clients » signifie recevoir et traiter les données qu'ils envoient,
// les requêtes, puis envoyer d'éventuelles réponses.
// Souvenez-vous aussi que send et surtout recv sont bloquants.
// La fonction surveille des ensembles de descripteurs de fichiers et plus particulierement
// readfds, writefds, et exceptfds.

// select retourne le nombre de sockets qui sont prêts à lire, écrire ou ayant une erreur,
// peut retourner 0 si aucun socket n'est prêt. Retourne -1 en cas d'erreur.

// Pour vérifier qu'une connexion entrante est en attente, que l'appel à accept ne sera pas bloquant,
// on doit vérifier que notre socket serveur est prêt en écriture :

// select verifie si des donnes sont dispo en lecture , ecruiture sur notre socket et retourne le nombre
bool Server::mySelect(fd_set &rd, fd_set &wr)
{
	FD_ZERO(&rd);
	FD_ZERO(&wr);
	FD_SET(_socket_server, &rd); // ajoute mon fd de serveur a lensemble
	FD_SET(_socket_server, &wr);

	std::vector<Client *>::iterator it;
	for (it = _client.begin(); it != _client.end(); it++)
	{
		if ((*it)->getSocketClient()) // n existe pas au 1er tour de boucle
		{
			FD_SET((*it)->getSocketClient(), &rd);
			FD_SET((*it)->getSocketClient(), &wr);
		}
	}
	int select_ready = select(FD_SETSIZE, &rd, &wr, NULL, NULL);
	if (select_ready == -1)
	{
		perror("select");
		if (_flag_keep_loop == false)
			return false;
		else
		{
			for (size_t i = 0; i < _client.size(); i++)
			{
				_client[i]->setFlagMustShutClient(true);
			}
			for (size_t i = 0; i < _channels.size(); i++)
			{
				_channels[i]->set_flag_erase_chan(true);
			}
			update();
		}
	}
	return (true);
}

void Server::myrecv(Client *client)
{
	char buf[1024] = {0};

	int res_rd = recv(client->getSocketClient(), buf, sizeof(buf), 0);
	if (res_rd < 0)
	{
		perror("receive client failed");
		close(client->getSocketClient());
		return;
	}
	else if (res_rd == 0)// ctrl C - nc
	{
		client->setFlagMustShutClient(true);
		client->set_arg_0();
		client->quit("");
		return;
	}
	
	if (buf[0])
	{
		INFO("=>Recois un message depuis le client "
			 << client->getSocketClient() << ": " << std::endl);
		std::cout << YELLOW_TXT << buf << RESET_TXT << std::endl;
		client->setMsgRecv(buf);
		client->setMsgRecvSave(buf);
	}
}

void Server::mysend(Client *client)
{
	if (!client->getMessage().empty())
	{
		INFO("=>Repond au client:" << std::endl);
		INFO("=>Message envoye a client " << client->getSocketClient()
										  << ": " << client->getMessage() << std::endl);
		size_t res_send = send(client->getSocketClient(), client->getMessage().c_str(), client->getMessage().size(), 0);
		if (res_send != client->getMessage().size())
		{
			perror("send client failed");
			close(client->getSocketClient());
		}
		client->clearMessage(); // reinitialise le message , sinon boucle
	}
}

void Server::update()
{
	for (size_t i = 0; i < _client.size();)
	{
		if (_client[i]->getFlagMustShutClient() == true)
		{
			delete _client[i];
			_client.erase(_client.begin() + i);
		}
		else
			i++;
	}
	for (size_t i = 0; i < _channels.size();)
	{
		if (_channels[i]->get_flag_erase_chan() == true)
		{
			delete (_channels[i]);
			_channels.erase(_channels.begin() + i);
		}
		else
			i++;
	}
}

bool Server::loop_recept_send()
{
	fd_set rd, wr;

	while (_flag_keep_loop == true)
	{
		std::vector<Client *>::iterator it;

		if (mySelect(rd, wr) == false)
			return false;

		if (FD_ISSET(_socket_server, &rd)) // check si notre socket est pret a lire // recoi le client, et ces logs
		{
			INFO("=>Accept le nouvel entrant: ");
			if (AcceptSocketClient() == false)
				return false;
		}
		for (it = _client.begin(); it != _client.end(); it++)
		{
			Client *client = *it;

			if (FD_ISSET(client->getSocketClient(), &rd))
			{
				myrecv(client);
				check_vectors();

				while (client->getCmdLine())
				{
					client->setVectorClient(&_client);
					client->setVectorChan(&_channels);
					if (client->checkParams(_password) == false)
					{
						if (client->isAuthenticate() == true)
							parse_msg_recv(client);
					}
				}
			}
			if (FD_ISSET(client->getSocketClient(), &wr)) // check si notre socket est pret a ecrire
				mysend(client);
		}
		update();
	}
	return true;
}

void Server::check_vectors()
{
	DEBUG("vector _channels: ");
	std::vector<Channel *>::iterator it;
	for (it = _channels.begin(); it != _channels.end(); it++)
	{
		DEBUG((*it)->getName() << " ");
		std::vector<Client *> clients = (*it)->getClients();
		for (std::vector<Client *>::iterator cit = clients.begin(); cit != clients.end(); cit++)
		{
			DEBUG("  << " + (*cit)->get_nickname());
		}
	}
	DEBUG(std::endl);
	DEBUG("vector _clients: ");
	std::vector<Client *>::iterator it2;
	for (it2 = _client.begin(); it2 != _client.end(); it2++)
		DEBUG((*it2)->getSocketClient() << " ");
	DEBUG(std::endl);
}