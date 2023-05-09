#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "irc.hpp"

class Channel;
class Client
{
public:

	//__________________________________________________canonic form

	Client(int sock_client);


	~Client(void);

	//__________________________________________________GETTERS_SETTERS	

	int							getSocketClient( void ) const;
	void						setSocketClient( int socket );

	void						setMsgRecv( std::string buf );
	std::string					getMsgRecv( void )const;

	void						setMsgRecvSave( std::string buf );
	std::string					getMsgRecvSave( void )const;
	
	void						setMessage( std::string );
	void						clearMessage( void );
	std::string					getMessage( void ) const;

	void		 				setFlagPsswdProvided(bool boolean);
	void				 		setFlagPsswd(bool boolean);

	std::string					get_mode( void );
	void						set_mode(std::string mode);

	bool				 		getFlagMustShutClient();

	void 						set_arg( void );
	std::vector<std::string>  	get_arg( void ) const;
	
	std::string					get_cmd( void ) const;
	
	void 						add_chan_ope( Channel *chan );
	std::vector<Channel*>	  	get_chan_ope( void ) const;

	
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
	bool						NicknameIsValid();
	void						Nick(std::string const&);
	bool						checkNick();
	// bool						checkSameNick(std::string const &old_nick);
	bool						checkSameNick();
	void						changeNick(std::string const& old_nick);
	void						checkUser( std::string const& );
	void 						clean_ping_mode(std::string const &arg);
	void 						Clean_arg( void );
	void						quit(std::string const &);
	void						broadcaster(std::string const& reply);
	void 						authenticationValid();
	bool						isAuthenticate();
	void						mode(std::string const &);



	void						setVectorClient(std::vector<Client*> *_ptr_client);

	void						setVectorChan(std::vector<Channel*> *_ptr_chan);

	void 						deleteOperator(Channel *chan);
	bool 						is_operator(Channel *chan);
	Channel						*search_chan(std::string chan);
	

private:
	// useless constructor
	Client(void);
	Client(Client const &cpy);
	Client &operator=(Client const &rhs);

	//attributes________________________________________________________________
	Message 					_message;
	// Connection			_msg_connection;			


	// Client	_client_data;	//données à récupérer et à stocker ? container: set ?
	int 						_socket_client;
	int							_step_registration; //compteur des étapes d'authentification
	bool						_flag_password_ok;
	bool						_flag_password_provided;
	bool						_flag_shut_client;
	bool						_pass_ok;
	bool						_nick_ok;
	bool						_user_ok;
	bool						_flag_not_registered;
	bool						_already_auth;
	std::string					_message_recv; // modifiee apres getcmdline
	std::string					_message_recv_save; //non modifiee apres getcmdline
	std::string 				_cmd_registration;
	std::string					_user;
	std::string					_nickname;
	std::string					_hostname;
	std::string					_realname;
	std::string					_mode; // "+i ou -i"
	
	std::vector<std::string>	_arg_registration;
	std::vector<Client*>		*_client;
	std::vector<Channel*>		*_channels;
	std::vector <Channel *>		_chan_ope;

};

#include "channel.hpp"
#endif