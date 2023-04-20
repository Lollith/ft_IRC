#include "irc.h"

//__________________________________________________canonic form

Client::Client(void):  _step_registration(0),_flag_password_ok(false), _flag_password_provided(false),
	_flag_shut_client(false), _user(""), _nickname(""), _hostname("")
{
}

Client::Client(int sock_client): 
	_socket_client(sock_client), _step_registration(0), _flag_password_ok(false), _flag_password_provided(false),
		_flag_shut_client(false), _user(""), _nickname(""), _hostname("")

{	
// 	std::cout << "create client" << std::endl;
// 
}

// Client::Client(Client const &cpy)
// {
// }

// Client &Client::operator=(Client const &rhs)
// {
// }

Client::~Client(void) {std::cout << YELLOW_TXT << "Client destructor called" << RESET_TXT << std::endl;} // close() ou/et freeinfo() à faire?

//__________________________________________________GETTERS_SETTERS

int Client::getSocketClient() const
{
	return this->_socket_client;
}

void Client::setSocketClient(int socket)
{
	this->_socket_client = socket;
}

std::string Client::getMessage(void) const
{
	return this->_message.getBuffer();
}

void Client::setMessage(std::string buffer)
{
	this->_message.setBuffer(buffer);
}

void Client::setMsgRecv( std::string buf )
{
	_message_recv = buf;
}

std::string Client::getMsgRecv( void ) const
{
	return this->_message_recv;
}

void Client::setMsgRecvSave( std::string buf )
{
	_message_recv_save = buf;
}

std::string Client::getMsgRecvSave( void ) const
{
	return this->_message_recv_save;
}

void	Client::setFlagPsswd( bool boolean )
{
	this->_flag_password_ok = boolean;
}

void	Client::setFlagPsswdProvided( bool boolean )
{
	this->_flag_password_provided = boolean;
}

std::vector<std::string> Client::get_arg( void ) const{
	return this->_arg_registration;
}

bool	Client::getFlagMustShutClient ()
{
	return this->_flag_shut_client;
}


void Client::set_arg( void ){
	std::vector<std::string>::iterator it = _arg_registration.begin();
	while( it != _arg_registration.end()) 
			_arg_registration.erase(it);
}

std::string Client::get_user( void ) const{
	return this->_user;
}

std::string Client::get_hostname( void ) const{
	return this->_hostname;
}

std::string Client::get_nickname( void ) const{
	return this->_nickname;
}

void	Client::set_nickname(std::string const& msg_rcv)
{
	size_t begin;
	size_t end;

	std::cout << RED_TXT << "message rec is: " << msg_rcv <<  RESET_TXT << std::endl;
	std::cout << "nickname: " << _nickname << std::endl;
	begin = msg_rcv.find("NICK", 0) + 5;
	if (begin != std::string::npos)
	{
		std::cout << "NICK NOT FOUND: " << _nickname <<  std::endl;

		end = msg_rcv.find("\r\n", begin);
		this->_nickname = msg_rcv.substr(begin, (end - begin));
	}
	else
		return;
}
//__________________________________________________MEMBERS FUNCTIONS


void Client::tokenization_cmd(std::string &cmd)
{
	size_t pos;
	size_t i = 0;

	pos = cmd.find(' ', i);
	this->_cmd_registration = cmd.substr(i, (pos - i));
	while (pos != std::string::npos)
	{
		i = pos + 1;
		pos = cmd.find(' ', i);
		this->_arg_registration.push_back(cmd.substr(i, (pos - i)));
	}
	// vérifs attributs cmd and args
	std::cout << "in tokenization: cmd is : " << this->_cmd_registration << std::endl;
	
	std::vector<std::string>::iterator it;	
	for (it = _arg_registration.begin(); it != _arg_registration.end(); it++) // 1er n hexiste pas , ne rentre pas
		std::cout << "in tokenization: args are : " << (*it) << std::endl;
}

void Client::ignoreCap(std::string const &)
{
	std::cout << GREEN_TXT << "here is CAP check func" << RESET_TXT << std::endl;

	this->_step_registration += 1;
}

// FERMER LE SOCKET CLIENT SI PASSWD FAUX + GARDER LE SERVEUR ALIVE
void Client::checkPassword(std::string const &psswd)
{
	std::cout << GREEN_TXT << "here is PASS check func" << RESET_TXT << std::endl;

	std::string rpl;
	
	this->_flag_password_provided = true;

	if (_arg_registration.back() == psswd)
	{
		this->_step_registration += 1;
		this->_flag_password_ok = true;
		std::cout << GREEN_TXT <<  "PASSWORD OK : " << _flag_password_ok << RESET_TXT << std::endl;
	}
	else if (_flag_password_ok == true)
	{
		setMessage(reply(ERR_ALREADYREGISTERED, this));
		return;
	}
	else if ((_cmd_registration == "PASS") && (_arg_registration.empty()))
	{
		rpl = reply(ERR_NEEDMOREPARAMS, this);
		rpl+= "ERROR: Server closing a client connection because need registration.\r\n";
		setMessage(rpl);
		_step_registration = 0;
		_flag_shut_client = true;
		return;
	}
	else
	{
		rpl = reply(ERR_PASSWDMISMATCH, this);
		rpl+= "ERROR: Server closing a client connection because need registration.\r\n";
		setMessage(rpl);
		_step_registration = 0;
		_flag_shut_client = true;
		return;
	}
}

void Client::checkNick(std::string const &)
{
	std::cout << GREEN_TXT << "here is NICK check func" << RESET_TXT << std::endl;

	this->_step_registration += 1;
}

void Client::checkUser(std::string const &)
{
	std::cout << GREEN_TXT <<"here is USER check func" << RESET_TXT << std::endl;

	this->_step_registration += 1;
	_user = _arg_registration[3];
	_hostname = _arg_registration[5];
	// _nickname = _arg_registration[2];

	if (_step_registration == 4)
	{
		std::string  buffer = "001 " + get_user() + " :Welcome to the " + _hostname + " Network, " + _nickname+"[!" + _user + "@" + _hostname + "]\r\n";
		_message.setBuffer(buffer);
	}

}

void Client::clean_ping_mode(std::string const &arg)
{
	(void) arg;
		Clean_arg();
}

void Client::checkParams(std::string const &password)
{
	int i = 0;
	std::string rpl;

	void (Client::*func_list[6])(std::string const &arg) =
		{&Client::ignoreCap, &Client::checkPassword, &Client::checkNick, &Client::checkUser, &Client::clean_ping_mode, &Client::clean_ping_mode};
	std::string cmd_to_check[6] = {"CAP", "PASS", "NICK", "USER", "PING", "MODE"};
	while (i < 5)
	{
		if (_cmd_registration == cmd_to_check[i])
		{
			if (i > 1 && _flag_password_provided == false)
			{
				rpl = reply(ERR_NOTREGISTERED, this);
				rpl+= "ERROR: Server closing a client connection because need registration.\r\n";
				setMessage(rpl);
				_flag_shut_client = true;
				_step_registration = 0;
				return;
			}
			else
			{
				(this->*(func_list[i]))(password);
				break;
			}
			
		}
		i++;
	}
}

void Client::Clean_arg()
{
	std::vector<std::string>::iterator it = _arg_registration.begin();
	while( it != _arg_registration.end()) 
		_arg_registration.erase (it);
}

void Client::getCmdLine(std::string const &password)
{
	const std::string eol_marker = "\r\n"; // à mettre ds un define?

	size_t pos;
	std::string cmd_line;

	if (_step_registration == 0)
	{
		std::cout << BLUE_TXT << "here set_nickname" << RESET_TXT << std::endl;
		set_nickname(_message_recv_save);
	}

	pos = this->_message_recv.find(eol_marker);
	while (pos != std::string::npos)
	{
		cmd_line = _message_recv.substr(0, pos);
		tokenization_cmd(cmd_line);
		checkParams(password);
		_message_recv.erase(_message_recv.begin(), (_message_recv.begin() + pos + eol_marker.length()));
		pos = this->_message_recv.find(eol_marker);
	}
}
