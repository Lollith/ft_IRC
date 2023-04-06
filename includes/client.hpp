#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "irc.h"

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

	void				setMsgRecv( std::string buf);

	std::string			getMessage( void ) const;

	//define typedef ici pour struct sockaddr_in??

private:
	// useless constructor

	//attributes________________________________________________________________
	Message 			_message;
	Connection			_msg_connection;			


	// Client	_client_data;	//données à récupérer et à stocker ? container: set ?
	int 				_socket_client; // faire un containeur
	std::string			_message_recv;
};

#endif