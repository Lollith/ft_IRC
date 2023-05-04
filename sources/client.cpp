#include "irc.hpp"

//__________________________________________________canonic form

Client::Client(void) : _step_registration(0), _flag_password_ok(false), _flag_password_provided(false),
					   _flag_shut_client(false), _cap_ok(false), _pass_ok(false), _nick_ok(false), _user_ok(false),
					   _flag_not_registered(false), _already_auth(false), _user(""), _nickname(""), _hostname("")
{
}

Client::Client(int sock_client) : _socket_client(sock_client), _step_registration(0), _flag_password_ok(false),
								  _flag_password_provided(false), _flag_shut_client(false),
								  _cap_ok(false), _pass_ok(false), _nick_ok(false), _user_ok(false),
								  _flag_not_registered(false), _already_auth(false), _user(""), _nickname(""), _hostname("")

{
	// 	std::cout << "create client" << std::endl;
	// _chan_ope = false;
	//
}

// Client::Client(Client const &cpy)
// {
// }

// Client &Client::operator=(Client const &rhs)
// {
// }

Client::~Client(void)
{
	std::cout << YELLOW_TXT << "Client destructor called" << RESET_TXT << std::endl;
	shutdown(_socket_client, SHUT_RDWR);
	close(_socket_client);
}

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

void Client::clearMessage()
{
	this->_message.resetBuffer();
}

void Client::setMsgRecv(std::string buf)
{
	_message_recv = buf;
}

std::string Client::getMsgRecv(void) const
{
	return this->_message_recv;
}

void Client::setMsgRecvSave(std::string buf)
{
	_message_recv_save = buf;
}

std::string Client::getMsgRecvSave(void) const
{
	return this->_message_recv_save;
}

void Client::setFlagPsswd(bool boolean)
{
	this->_flag_password_ok = boolean;
}

void Client::setFlagPsswdProvided(bool boolean)
{
	this->_flag_password_provided = boolean;
}

std::vector<std::string> Client::get_arg(void) const
{
	return this->_arg_registration;
}

void Client::set_arg(void)
{
	std::vector<std::string>::iterator it = _arg_registration.begin();
	while (it != _arg_registration.end())
		_arg_registration.erase(it);
}

std::string Client::get_cmd(void) const
{
	return (this->_cmd_registration);
}

bool Client::getFlagMustShutClient()
{
	return this->_flag_shut_client;
}

std::string Client::get_user(void) const
{
	return this->_user;
}

std::string Client::get_hostname(void) const
{
	return this->_hostname;
}

std::string Client::get_nickname(void) const
{
	return this->_nickname;
}

void Client::setVectorClient(std::vector<Client *> *clients)
{
	_client = clients;
}

void Client::setVectorChan(std::vector<Channel *> *ptr_chan)
{
	_channels = ptr_chan;
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
	this->_cap_ok = true;
}

// FERMER LE SOCKET CLIENT SI PASSWD FAUX + GARDER LE SERVEUR ALIVE
void Client::checkPassword(std::string const &psswd)
{
	std::cout << GREEN_TXT << "here is PASS check func" << RESET_TXT << std::endl;

	std::string rpl;

	this->_flag_password_provided = true;

	if (_arg_registration.back() == psswd)
	{
		this->_pass_ok = true;
		this->_step_registration += 1;
		this->_flag_password_ok = true;
		std::cout << GREEN_TXT << "PASSWORD OK : " << _flag_password_ok << RESET_TXT << std::endl;
	}
	else if (isAuthenticate())
	{
		setMessage(reply(ERR_ALREADYREGISTERED, this));
		return;
	}
	else if ((_cmd_registration == "PASS") && (_arg_registration.empty()))
	{
		rpl = reply(ERR_NEEDMOREPARAMS, this);
		rpl += "ERROR: Server closing a client connection because need registration.\r\n";
		setMessage(rpl);
		_flag_shut_client = true;
		return;
	}
	else
	{
		rpl = reply(ERR_PASSWDMISMATCH, this);
		rpl += "ERROR: Server closing a client connection because need registration.\r\n";
		setMessage(rpl);
		_flag_shut_client = true;
		return;
	}
}

bool Client::NicknameIsValid()
{
	if (_nickname.length() >= 30)
		return false;

	if (_nickname.find(' ') != std::string::npos || _nickname.find(',') != std::string::npos || _nickname.find('.') != std::string::npos || _nickname.find('*') != std::string::npos || _nickname.find('?') != std::string::npos || _nickname.find('!') != std::string::npos || _nickname.find('@') != std::string::npos || _nickname.empty())
	{
		return false;
	}
	if (_nickname[0] == '$' || _nickname[0] == ':' || _nickname[0] == '#' || _nickname[0] == '&')
	{
		return false;
	}
	else
		return true;
}

bool Client::checkSameNick()
{
	size_t i = 0;
	while ((_client->size() > 1) && (i != _client->size() - 1)) // broadcast the messag
	{
		if ((*_client)[i] != this && (*_client)[i]->get_nickname() == this->_nickname)
		{
			std::cout << BLUE_TXT << "differents clients have same nickname" << RESET_TXT << std::endl;
			setMessage(reply(ERR_NICKNAMEINUSE, this));
			return true;
		}
		i++;
	}
	return false;
}

void Client::changeNick(std::string const &old_nick)
{
	if (this->_pass_ok == true)
	{
		std::string message = ":";
		if (old_nick.size() && get_user().size() && get_hostname().size())
			message += old_nick + "!" + get_user() + "@" + get_hostname();
		message += " NICK :" + _nickname + "\r\n";
		broadcaster(message);
	}
	else
		return;
}

void Client::Nick(std::string const &)
{
	std::cout << GREEN_TXT << "here is NICK func" << RESET_TXT << std::endl;
	if (_arg_registration.empty())
	{
		setMessage(reply(ERR_NONICKNAMEGIVEN, this));
		_flag_shut_client = true;
		return;
	}
	std::string old_nick = _nickname;
	std::cout << "old nickname= " << old_nick << std::endl;
	this->_nickname = _arg_registration.back();
	std::cout << "new nickname= " << _nickname << std::endl;
	if (!NicknameIsValid())
	{
		std::cout << BLUE_TXT << "condition nickname not valid should respond" << RESET_TXT << std::endl;
		std::cout << _nickname << std::endl;
		setMessage(reply(ERR_ERRONEUSNICKNAME, this));
		_flag_shut_client = true;
		return;
	}
	if (checkSameNick() == true)
	{
		std::cout << CYAN_TXT << "here, same nicks == true" << RESET_TXT << std::endl;
		if (isAuthenticate())
			_nickname = old_nick; // revenir en  arrière en cas de nick already use
		return;
	}
	if (_nick_ok == false)
	{
		this->_nick_ok = true;
		this->_step_registration += 1;
	}
	changeNick(old_nick);
}

void Client::checkUser(std::string const &)
{
	std::cout << GREEN_TXT << "here is USER check func" << RESET_TXT << std::endl;

	if (this->_already_auth == true && this->_flag_password_provided == true)
	{
		setMessage(reply(ERR_ALREADYREGISTERED, this));
		return;
	}
	if (_arg_registration.size() < 4)
	{
		setMessage(reply(ERR_NEEDMOREPARAMS, this));
		return;
	}

	this->_user_ok = true;
	this->_step_registration += 1;
	_user = _arg_registration[1];
	_hostname = _arg_registration[2];

	std::string res;
	size_t pos = 0;

	for (size_t i = 0; i != _arg_registration.size(); i++)
	{
		if (_arg_registration[i].find_first_of(':', 0) != std::string::npos)
			pos = i;
	}
	res = _arg_registration[pos];
	pos++;
	for (; pos != _arg_registration.size(); pos++)
	{
		res += " " + _arg_registration[pos];
	}
	this->_realname = res;
	std::cout << BLUE_TXT << "realname is ->" << _realname << RESET_TXT << std::endl;
}

void Client::clean_ping_mode(std::string const &)
{
	std::string msg = ":" + get_nickname() + "!" + get_user() + "@" + get_hostname() + " PONG " + this->_arg_registration.back() + "\r\n";
	setMessage(msg);
}

void Client::quit(std::string const &)
{
	INFO("HERE QUIT FUNC\n");
	std::string res;
	std::string quit_reason;
	std::string broadcast_rpl;

	// récupere le parametre apres les ´:´ (reason param)
	size_t pos = 0;
	for (size_t i = 0; i != _arg_registration.size(); i++)
	{
		if (_arg_registration[i].find_first_of(':', 0) != std::string::npos)
			pos = i;
	}
	res = _arg_registration[pos];
	pos++;
	for (; pos != _arg_registration.size(); pos++)
	{
		res += " " + _arg_registration[pos];
	}
	quit_reason = res;

	setMessage("ERROR: Server closing a client connection\r\n");
	std::vector<Channel *>::iterator it_chan;
	for (it_chan = this->_channels->begin(); it_chan != _channels->end(); it_chan++)
	{
		if ((*it_chan)->has_clients(this))
		{
			broadcast_rpl = ":" + get_nickname() + "!" + get_user() + "@" + get_hostname() + " QUIT :" + "QUIT " + quit_reason + "\r\n";
			std::vector<Client *> vectclients = (*it_chan)->getClients();
			std::vector<Client *>::iterator it_client;
			for (it_client = vectclients.begin(); it_client != vectclients.end(); it_client++)
			{
				if (*it_client != this) // do not send the message channels times to this
					(*it_client)->setMessage(broadcast_rpl);
			}
			((*it_chan)->deleteClientFromChan(this));
			if ((*it_chan)->getClients().size() < 1)
				(*it_chan)->set_flag_erase_chan(true);
			this->_flag_shut_client = true;
			return;
		}
		else
		{
			setMessage(reply(ERR_NOTONCHANNEL, this, (*it_chan)));
			return;
		}
	}
}

void Client::checkParams(std::string const &password)
{
	int i = 0;
	int nb_func = 6;
	std::string rpl;

	void (Client::*func_list[])(std::string const &arg) =
		{&Client::ignoreCap, &Client::checkPassword, &Client::Nick, &Client::checkUser,
		 &Client::clean_ping_mode, &Client::quit};
	std::string cmd_to_check[] = {"CAP", "PASS", "NICK", "USER", "PING", "QUIT"};
	while (i < nb_func)
	{
		if (_cmd_registration == cmd_to_check[i])
		{
			if (i > 1 && _flag_password_provided == false && _flag_not_registered == false)
			{
				rpl = reply(ERR_NOTREGISTERED, this);
				rpl += "ERROR: Server closing a client connection because need registration.\r\n";
				setMessage(rpl);
				_flag_shut_client = true;
				_flag_not_registered = true;
				break;
			}
			else
			{
				int before_step = this->_step_registration;
				(this->*(func_list[i]))(password);
				if (before_step != this->_step_registration)
					this->authenticationValid();
				break;
			}
		}
		i++;
	}
}

void Client::Clean_arg()
{
	std::vector<std::string>::iterator it = _arg_registration.begin();
	while (it != _arg_registration.end())
		_arg_registration.erase(it);
}

void Client::getCmdLine(std::string const &password)
{
	const std::string eol_marker = "\r\n"; // à mettre ds un define?

	size_t pos;
	std::string cmd_line;

	pos = this->_message_recv.find(eol_marker);
	while (pos != std::string::npos)
	{
		cmd_line = _message_recv.substr(0, pos);
		set_arg();
		tokenization_cmd(cmd_line);
		checkParams(password);
		_message_recv.erase(_message_recv.begin(), (_message_recv.begin() + pos + eol_marker.length()));
		pos = this->_message_recv.find(eol_marker);
	}
}

void Client::broadcaster(std::string const &reply)
{
	std::vector<Channel *>::iterator it_chan;
	for (it_chan = this->_channels->begin(); it_chan != _channels->end(); it_chan++)
	{
		if ((*it_chan)->has_clients(this))
		{
			std::vector<Client *> vectclients = (*it_chan)->getClients();
			std::vector<Client *>::iterator it_client;
			for (it_client = vectclients.begin(); it_client != vectclients.end(); it_client++)
			{
				if (*it_client != this) // do not send the message channels times to this
					(*it_client)->setMessage(reply);
			}
		}
	}
	setMessage(reply);
}

bool Client::isAuthenticate()
{
	if (_cap_ok == true && _pass_ok == true && _nick_ok == true && _user_ok == true)
		return true;
	else
		return false;
}

void Client::authenticationValid()
{
	if (isAuthenticate() && _already_auth == false)
	{
		std::string buffer = ":" + get_nickname() + "!" + get_user() + "@" + get_hostname() + " 001 " + get_nickname() + " :Welcome to the " + _hostname + " Network " + _nickname + "!" + _user + "@" + _hostname + "\r\n";
		_message.setBuffer(buffer);
		_already_auth = true;
	}
}