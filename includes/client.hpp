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

	void						setMsgRecv( std::string buf );
	std::string					getMsgRecv( void )const;

	void						setMsgRecvSave( std::string buf );
	std::string					getMsgRecvSave( void )const;
	
	std::string					getMessage( void ) const;
	void						setMessage( std::string );

	void				 		setFlagPsswd(bool boolean);
	void		 				setFlagPsswdProvided(bool boolean);

	std::vector<std::string>  	get_arg( void ) const;
	void 						set_arg( void );

	std::string					get_user( void ) const;
	std::string					get_hostname( void ) const;
	std::string					get_nickname( void ) const;


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
	void 						clean_ping_mode(std::string const &arg);
	void 						Clean_arg( void );
	//define typedef ici pour struct sockaddr_in??
	





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
	std::string					_message_recv; // modifiee apres getcmdline
	std::string					_message_recv_save; //non modifiee apres getcmdline
	std::string 				_cmd_registration;
	std::string					_user;
	std::string					_nickname;
	std::string					_hostname;
	// std::string					_arg0;
	
	std::vector<std::string>	_arg_registration;

};

#include "channel.hpp"
#endif