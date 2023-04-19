#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "irc.h"

class Channel;
class Client
{
public:
	//__________________________________________________canonic form

	Client(int sock_client);

	// Client(Client const &cpy);

	// Client &operator=(Client const &rhs);

	~Client(void);

	//__________________________________________________GETTERS_SETTERS	

	int							getSocketClient( void ) const;
	void						setSocketClient( int socket );

	std::string					getMsgRecv( void )const;
	void						setMsgRecv( std::string buf );

	std::string					getMessage( void ) const;
	void						setMessage( std::string );

	void		 				setFlagPsswdProvided(bool boolean);
	void				 		setFlagPsswd(bool boolean);

	std::vector<std::string>  	get_arg( void ) const;
	std::string					get_user( void ) const;
	std::string					get_hostname( void ) const;

	std::string					get_nickname( void ) const;
	void						set_nickname(std::string msg_rcv);


	//__________________________________________________MEMBERS
	// dans cette fonction split du buf pour récuperer une pair commande - argument
	void 						tokenization_cmd( std::string& cmd_line );
	void 						getCmdLine( std::string const& password );

	//here fonctions qui vont check d'éventuelles d'erreurs et construire un objet
	// Conncexioin pour envoyer la réponse adéquate
	void 						checkParams( std::string const& password );
	void						ignoreCap( std::string const& );
	void						checkPassword( std::string const& password );
	void						checkNick( std::string const& );
	void						checkUser( std::string const& );

	//define typedef ici pour struct sockaddr_in??
	




	std::vector<std::string>	_arg_registration;

private:
	// useless constructor
	Client(void);

	//attributes________________________________________________________________
	Message 			_message;
	// Connection			_msg_connection;			


	// Client	_client_data;	//données à récupérer et à stocker ? container: set ?
	int 						_socket_client;
	int							_step_registration; //compteur des étapes d'authentification
	bool						_flag_password_ok;
	bool						_flag_password_provided;
	std::string					_message_recv;
	std::string 				_cmd_registration;
	std::string					_user;
	std::string					_nickname;
	std::string					_hostname;

};

#include "channel.hpp"
#endif