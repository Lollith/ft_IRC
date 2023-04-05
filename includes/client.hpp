#ifndef CLIENT_HPP
#define CLIENT_HPP

class Client
{
public:
	//__________________________________________________canonic form

	Client(int sock_client);
	// Client(Client const &cpy);

	// Client &operator=(Client const &rhs);

	~Client(void);

	//__________________________________________________GETTERS_SETTERS	
	int					getSocketClient() const;
	void				setSocketClient(int socket);

	// int					getPort();

	// std::string			getPassword();

	//define typedef ici pour struct sockaddr_in??

private:
	// useless constructor
	Client(void);

	//attributes________________________________________________________________

	int					_port;
	int 				_socket_client; // faire un containeur
	// struct sockaddr_in	_client_addr; // info adresse du client // ici ?
	// std::string 		_password;
	// Messages _messages;		//class Messages ?
	// Client	_client_data;	//données à récupérer et à stocker ? container: set ?



};

#endif