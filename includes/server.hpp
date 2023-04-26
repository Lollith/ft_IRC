#ifndef SERVER_HPP
#define SERVER_HPP

#include "irc.h"

#define BACKLOG 10

class Client;
class Channel;
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

	bool 				AcceptSocketClient();

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
	void				mySelect(fd_set &rd, fd_set &wr);
	void  				myrecv(Client *client);
	void 				mysend(Client *client);
	bool				loop_recept_send();


	//-----fct _channels------------------------------------------------------------
	void 				parse_msg_recv( Client *client, std::string msg_recv ); // msg_recv et arg useless => a suprimer
	void 				join( Client *client);
	void 				quit( Client *client);
	void				privmsg( Client *client);
	void				changeNick( Client *client);

	void 				stop();
	void 				welcome_new_chan( Client *client, Channel *channel );
	void 				names( Client *client );

private:
	// useless constructor
	Server(void);

	//attributes________________________________________________________________

	int						_port;
	int						_socket_server; //listen sur cette socket
	struct sockaddr_in		_addr_server;
	struct sockaddr_in		_client_addr; // info adresse du client // ici ?
	socklen_t				_sin_size;
	std::string 			_password;
	bool					_flag_keep_loop;
	
	std::vector<Client*>	_client;
	std::vector<Channel*>	_channels;
};

#include "irc.h"
#endif