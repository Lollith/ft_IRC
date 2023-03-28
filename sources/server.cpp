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
	//test
	// else
	// 	std::cout << "first socket ok" << std::endl;
	//test
	setAddrServ();
}

// Server::Server(Server const &cpy):
// {

// }

// Server &Server::operator=(Server const &rhs)
// {

// }

Server::~Server(void) {} // close? freeinfo?

//__________________________________________________GETTERS_SETTERS

// sockets_________________________________________________________________

// creer un socket -descirpteur de fichier
// https://man7.org/linux/man-pages/man2/socket.2.html
// AF_INET: IPv4, SOCK_STREAM :TCP, IPPPROTO_TCP :TCP => 0 Indicates
// that the default protocol for the type selected is to be used. For example,
// IPPROTO_TCP is chosen for the protocol if the type was set to SOCK_STREAM
// and the address family is AF_INET.
//__________________________________________________________________________

// return false si la création de la socket a échoué
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

bool Server::setSocketClient()
{
	this->_socket_client = accept(_socket_server, (struct sockaddr *)&_client_addr, &_sin_size);
	if (this->_socket_client == -1)
	{
		perror("accept()");
		return false;
	}
	return true;
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
	// associer la socket avec un port de votre machine locale.

	this->_addr_server.sin_family = AF_INET;		  // host byte order
	this->_addr_server.sin_port = htons(this->_port); // short, network byte order
	this->_addr_server.sin_addr.s_addr = INADDR_ANY;  // auto-remplissage avec  mon IP//inet_addr("127.0.0.1");
	bzero(&(this->_addr_server.sin_zero), 8);		  // interdite?
}

// define typedef ici pour struct sockaddr_in??
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

// return false en cas d'erreur
bool Server::startServer()
{
	char buf[3];
	// , nous devons appeler bind() avant d'appeler listen() ou sinon le système va écouter sur un port au hasard.
	// int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
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
		return (1);
	}
	while (1)
	{
		setSinSize();
		if (setSocketClient())
		{
			int res_send = send(_socket_client, "HI\n", 3, 0);
			if ( res_send < 0)
			{
				perror("send client failed");
				return 1;
			}
			int res_recv = recv(_socket_client, buf, 3, 0);
			if (res_send != res_recv)
			{
				perror("bad data len, send bytes and recv bytes not matching");
				return 1;
			}
			if (res_recv < 0)
			{
				perror("receive client failed");
				return 1;
			}
		}
	
	}
	return true;
}
