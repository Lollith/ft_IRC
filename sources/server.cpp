#include "irc.h"

//__________________________________________________canonic form

Server::Server(const int port, const std::string password)
	: _port(port), _password(password), _flag_keep_loop(true)
{
	if (!setSocketServer())
	{
		std::cout << "cannot create server object because it is not possible to create socket server."
				  << std::endl;
		return;
	}
	setAddrServ();
}

// Server::Server(Server const &cpy)
// {
// }

// Server &Server::operator=(Server const &rhs)
// {
// }

Server::~Server(void)
{
	std::cout << YELLOW_TXT << "Destructor server called." << RESET_TXT << std::endl;

	shutdown(this->_socket_server, SHUT_RDWR);
	close(this->_socket_server);
	std::vector<Client *>::iterator it;
	for (it = _client.begin(); it != _client.end(); it++)
	{
		// // à modifier?_____________________________
		shutdown((*it)->getSocketClient(), SHUT_RDWR);
		close((*it)->getSocketClient());
		//________________________________________
		// delete (*it);
	}
	_client.clear();
} // close() ou/et freeinfo() à faire?

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

	std::cout << socket << std::endl;	
		_client.push_back(new Client(socket));

	return true;
}

// int Server::getSocketClient()
// {
// 	return Client::_socket_client;
// }

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
//bind= associer la socket avec un port de votre machine locale.
//listen = les connections vont attendre dans la file de BACKLOG jusqu'à ce qu' on
//les accept()
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

// envoi dun message dun client => serveur => client 2// passe tjs par le serveur qui renvoi
// doit etre connecter a un channel pour que les messages soient envoyes

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

bool Server::loop_recept_send()
{
	fd_set rd, wr;
	// std::vector<Client *>::iterator it;
	// for (it = _client.begin(); it != _client.end(); it++)
	// {
	// 	(*it)->setFlagPsswd(false);
	// 	(*it)->setFlagPsswdProvided(false);
	// }

	while (_flag_keep_loop == true)
	{
		char buf[1024] = {0};
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
			// return false; // continue?? si errno == eintr
		}
		// else if (select_ready == 0) //utile?
		// {
		// 	std::cout <<"timeout"<< std::endl;
		// 	continue;
		// }

		if (FD_ISSET(_socket_server, &rd)) // check si notre socket est pret a lire // recoi le client, et ces logs
		{
			INFO("=>Accept le nouvel entrant: \n");
			if (AcceptSocketClient() == false)
				return false;
		}

		for (it = _client.begin(); it != _client.end(); it++)
		{
			Client *client = *it;
			
			//----------------recev------------------------------------------------------------
			if (FD_ISSET(client->getSocketClient(), &rd))
			{
				int res_rd = recv(client->getSocketClient(), buf, sizeof(buf), 0);
				if (res_rd < 0)
				{
					perror("receive client failed");
					close(client->getSocketClient());
					return false;
				}
				if (buf[0])
				{
					std::cout << "=>Recois un message depuis le client "<< client->getSocketClient()<< ": "<< std::endl;
					std::cout << buf << std::endl;
					client->setMsgRecv(buf);
				}
		
				client->getCmdLine(_password);
				parse_msg_recv(client, buf); // client issu de mon vector de client
			}

			if (FD_ISSET(client->getSocketClient(), &wr)) // check si notre socket est pret a ecrire
			{
				if(!client->getMessage().empty()) // comme je reinitialise a la fin le message
				{
					std::cout << "=>Repond au client:" << std::endl;
					size_t res_send = send(client->getSocketClient(), client->getMessage().c_str(), client->getMessage().size(), 0);
					if (res_send != client->getMessage().size())
					{
						perror("send client failed");
						close(client->getSocketClient());
						// return false;
					}
					std::cout << "=>Message envoye: " << client->getMessage() <<", a client "<< client->getSocketClient()<< std::endl;
					client->setMessage(""); // reinitialise le message , sinon boucle
				}
			}	
		}
	}
	return true;
}

void Server::Clean_arg(Client *client)
{
		std::vector<std::string>::iterator it = client->get_arg().begin()+ 1;
		while( it != client->get_arg().end()) 
			client->get_arg().erase (it);
}