#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> //htons +define the in_addr cf <netinet/in.h>
#include "message.hpp"

class Client
{
public:
	//__________________________________________________canonic form

	Client(void);
	// Client(int sock_client);
	// Client(Client const &cpy);

	// Client &operator=(Client const &rhs);

	~Client(void);

	//__________________________________________________GETTERS_SETTERS	

	int					getSocketClient( void ) const;
	void				setSocketClient( int socket );

	std::string			getMessage( void ) const;

	//define typedef ici pour struct sockaddr_in??

private:
	// useless constructor

	//attributes________________________________________________________________
	Message 			_message;		


	// Client	_client_data;	//données à récupérer et à stocker ? container: set ?
	int 				_socket_client; // faire un containeur
};

#endif