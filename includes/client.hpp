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
#include <set>

class Client
{
public:
	//__________________________________________________canonic form

	Client(void);
	// Client(Client const &cpy);

	// Client &operator=(Client const &rhs);

	~Client(void);

	//__________________________________________________GETTERS_SETTERS	

	int					getSocketClient( void ) const;
	void				setSocketClient( int socket );

	void				setMsgRecv( std::string buf);

	std::string			getMessage( void ) const;

	// dans cette fonction split du buf pour récuperer une pair commande - argument
	// lancement d'une fonction de check
	void 				tokenization_cmd(std::string cmd_line);
	void 				getCmdLine();
	void 				checkParams();

	//define typedef ici pour struct sockaddr_in??

private:
	// useless constructor

	//attributes________________________________________________________________
	Message 			_message;
	Connection			_msg_connection;			


	// Client	_client_data;	//données à récupérer et à stocker ? container: set ?
	int 						_socket_client; // faire un containeur
	std::string					_message_recv;
	std::string 				_cmd_registration;
	std::vector<std::string>	_arg_registration;

};

#endif