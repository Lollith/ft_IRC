#include "server.hpp"

//__________________________________________________canonic form

Server::Server(const int port, const std::string password)
	: _port(port), _password(password)
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

Server::~Server(void) {} // close() ou/et freeinfo() à faire?

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
	this->_socket_server = socket(AF_INET,SOCK_STREAM, 0);
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

void Server::setSocketClient()
{
	setSinSize();
	this->_socket_client = accept(_socket_server, (struct sockaddr *)&_client_addr, &_sin_size);
	if (this->_socket_client == -1)
	{
		perror("accept()");
		// return false;
	}
	// return true;
}

int Server::getSocketClient()
{
	return this->_socket_client;
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
	this->_addr_server.sin_addr.s_addr = INADDR_ANY;  // auto-remplissage avec  mon IP//inet_addr("127.0.0.1");
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
	int	yes = 1;

	if (setsockopt(_socket_server, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt");
		exit(1);
	}
}

// return false en cas d'erreur
bool Server::startServer()
{
	crash_protector();
  
	// associer la socket avec un port de votre machine locale.
	if (bind(_socket_server, (struct sockaddr *)&_addr_server, sizeof(struct sockaddr)) == -1)
	{
		perror("bind");
		return false;
	}
	// les connections vont attendre dans la file de BACKLOG jusqu'à ce que vous
	// les acceptiez avec accept()
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
//dont le socket retourne une erreur indiquant qu'ils ont été déconnectés.

// « Gérer ses clients » signifie recevoir et traiter les données qu'ils envoient,
// les requêtes, puis envoyer d'éventuelles réponses.
// Souvenez-vous aussi que send et surtout recv sont bloquants.
// La fonction surveille des ensembles de descripteurs de fichiers et plus particulierement 
//readfds, writefds, et exceptfds.

// select retourne le nombre de sockets qui sont prêts à lire, écrire ou ayant une erreur, 
//peut retourner 0 si aucun socket n'est prêt. Retourne -1 en cas d'erreur.

// Pour vérifier qu'une connexion entrante est en attente, que l'appel à accept ne sera pas bloquant,
// on doit vérifier que notre socket serveur est prêt en écriture :

bool Server::loop_recept_send()
{
	fd_set rd,wr,er;
	timeval timeout = {0};
	FD_ZERO (&rd); // initialise
	FD_ZERO (&wr); 
	FD_ZERO (&er); 
	FD_SET (_socket_server, &rd);// ajoute mon fd de serveur a lensemble
	FD_SET (_socket_server, &wr);
	FD_SET (_socket_server, &er);
	//a faire : add all client to the set


	while (1)
	{
		char buf[1024] = {0};

		int select_ready = select(_socket_server + 1, &rd, &wr, &er, NULL); // select verifie si des donnes sont dispo en lecture , ecruiture sur notre socket et retourne le nombre
		std::cout << select_ready<< std::endl;
		if (select_ready == -1)
		{
			perror("select");
			return false; // continue?? si errno == eintr
		}
		if(FD_ISSET(_socket_server, &rd)) // check si notre socket est pret a lire
		{
		setSocketClient();//accept le client et lui associe une nouvel socket
			{
				int res_rd = recv(_socket_client, buf, sizeof(buf), 0);
				// int res_rd = read(_socket_client, buf, sizeof(buf));
				std::cout << buf << std::endl;

				// condition à changer en fonction de la taille de buf
				if (res_rd < 0) 
				{
					perror("receive client failed");
					return false;
				}
			}
		if(FD_ISSET(_socket_client, &wr)) // check si notre socket est pret a lire
		{
			int res_send = send(_socket_client, "HI\n", 3, 0);
				// il faut s'assurer que tout le buffer est envoyé et adapter cette condition
				// en attendant on le hard code
			if ( res_send != 3) 
			{
				perror("send client failed");
				return false;
			}

		}
		}
	}
	return true;
}
// a faire  vector de client => si accept => pushback ds le client le nouvel fd
// si serveur rrecoi => creer un nouveau client => nouveau client va lire ou ercrire