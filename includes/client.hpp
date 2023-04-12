#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "irc.h"

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
	std::string			getMsgRecv( void )const;

	std::string			getMessage( void ) const;
	void				setMessage( std::string );

	void		 		setFlagPsswd(bool boolean);
	void		 		setFlagPsswdProvided(bool boolean);


	// dans cette fonction split du buf pour récuperer une pair commande - argument
	void 	tokenization_cmd(std::string& cmd_line);
	void 	getCmdLine(std::string const& password);

	//here fonctions qui vont check d'éventuelles d'erreurs et construire un objet
	// Conncexioin pour envoyer la réponse adéquate
	void 	checkParams(std::string const& password);
	void	ignoreCap(std::string const&);
	void	checkPassword(std::string const& password);
	void	checkNick(std::string const&);
	void	checkUser(std::string const&);

	//define typedef ici pour struct sockaddr_in??
	
	//__________________________________________________MEMBERS





private:
	// useless constructor

	//attributes________________________________________________________________
	Message 			_message;
	Connection			_msg_connection;			


	// Client	_client_data;	//données à récupérer et à stocker ? container: set ?
	int 						_socket_client; // faire un containeur
	int							_step_registration; //compteur des étapes d'authentification
	bool						_flag_password_ok;
	bool						_flag_password_provided;
	std::string					_message_recv;
	std::string 				_cmd_registration;
	std::vector<std::string>	_arg_registration;

};

#endif