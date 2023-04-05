#ifndef SERVER_HPP
#define SERVER_HPP

#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> //htons +define the in_addr cf <netinet/in.h>
#include "client.hpp"

#define BACKLOG 10

class Server
{
public:
	//__________________________________________________canonic form

	Server(const int port, const std::string password);
	// Server(Server const &cpy);

	// Server &operator=(Server const &rhs);

	~Server(void);

	//__________________________________________________GETTERS_SETTERS	

	//return false si la création de la socket a échoué
	bool				setSocketServer();
	int					getSocketServer();

	int 				AcceptSocketClient();
	// int					getSocketClient();

	int					getPort();

	std::string			getPassword();

	//define typedef ici pour struct sockaddr_in??
	void				setAddrServ();
	struct sockaddr_in	getAddrServ();

	void 				setSinSize();
	socklen_t 			getSinSize();

	//__________________________________________________members functions

	void 				crash_protector();
	bool				startServer();
	bool				loop_recept_send();

private:
	// useless constructor
	Server(void);

	//attributes________________________________________________________________

	int					_port;
	int					_socket_server; //listen sur cette socket
	struct sockaddr_in	_addr_server;
	// int 				_socket_client; // ici ou dans une classe client? //connection sur le client
	struct sockaddr_in	_client_addr; // info adresse du client // ici ?
	socklen_t			_sin_size;
	std::string 		_password;
	// Messages _messages;		//class Messages ?
	// Client	_client_data;	//données à récupérer et à stocker ? class?
};

#endif