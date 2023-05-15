#include "irc.hpp"

//__________________________________________________canonic form

Client::Client(void) : _flag_erroneus(false), _step_registration(0), _flag_password_ok(false), _flag_password_provided(false),
					   _flag_shut_client(false), _pass_ok(false), _nick_ok(false), _user_ok(false),
					   _flag_not_registered(false), _already_auth(false),
					   _user(""), _nickname(""), _hostname("")
{
	_mode[I] = "+";
	_mode[O] = "-";
}

Client::Client(int sock_client) : _flag_erroneus(false), _socket_client(sock_client), _step_registration(0), _flag_password_ok(false),
								  _flag_password_provided(false), _flag_shut_client(false),
								  _pass_ok(false), _nick_ok(false), _user_ok(false),
								  _flag_not_registered(false), _already_auth(false),
								  _user(""), _nickname(""), _hostname("")

{
	_mode[I] = "+";
	_mode[O] = "-";
}

Client::Client(Client const &cpy)
{
	*this = cpy;
}

Client &Client::operator=(Client const &rhs)
{
	if (this != &rhs)
	{
		_message = rhs._message;
		_socket_client = rhs._socket_client;
		_step_registration = rhs._step_registration;
		_flag_password_ok = rhs._flag_password_ok;
		_flag_password_provided = rhs._flag_password_provided;
		_flag_shut_client = rhs._flag_shut_client;
		_pass_ok = rhs._pass_ok;
		_nick_ok = rhs._nick_ok;
		_user_ok = rhs._user_ok;
		_flag_not_registered = rhs._flag_not_registered;
		_already_auth = rhs._already_auth;
		_message_recv = rhs._message_recv;
		_message_recv_save = rhs._message_recv_save;
		_cmd_registration = rhs._cmd_registration;
		_user = rhs._user;
		_nickname = rhs._nickname;
		_hostname = rhs._hostname;
		_realname = rhs._realname;
		_mode[2] = rhs._mode[2];
		_arg_registration = rhs._arg_registration;
		_client = rhs._client;
		_channels = rhs._channels;
		_chan_ope = rhs._chan_ope;
	}
	return (*this);
}

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
	_message_recv += buf;
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

std::string *Client::get_mode(void)
{
	return _mode;
}

void Client::set_mode(std::string const &mode)
{
	if (mode == "+i" || mode == "-i")
		_mode[I] = mode[0]; // recup le + ou -
	if (mode == "+o" || mode == "-o")
		_mode[O] = mode[0]; // recup le + ou -
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

void Client::set_arg_0(void)
{
	_arg_registration[0] = "connexion lost";
}

std::string Client::get_cmd(void) const
{
	return (this->_cmd_registration);
}

void Client::setFlagMustShutClient(bool boolean)
{
	this->_flag_shut_client = boolean;
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

// FERMER LE SOCKET CLIENT SI PASSWD FAUX + GARDER LE SERVEUR ALIVE
void Client::checkPassword(std::string const &psswd)
{
	std::cout << GREEN_TXT << "here is PASS check func" << RESET_TXT << std::endl;

	std::string rpl;

	this->_flag_password_provided = true;

	if (!_arg_registration.empty())
	{
		if (_arg_registration.back() == psswd)
		{
			this->_pass_ok = true;
			this->_step_registration += 1;
			this->_flag_password_ok = true;
		}
		else
		{
			rpl = reply(ERR_PASSWDMISMATCH, this);
			rpl += "ERROR: Server closing a client connection because need registration.\r\n";
			setMessage(rpl);
			_flag_shut_client = true;
			return;
		}
		if (isAuthenticate())
		{
			setMessage(reply(ERR_ALREADYREGISTERED, this));
			return;
		}
	}
	else
	{
		rpl = reply(ERR_NEEDMOREPARAMS, this);
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
	if (this->_pass_ok == true && _arg_registration.empty())
	{
		setMessage(reply(ERR_NONICKNAMEGIVEN, this));
		return;
	}
	std::string old_nick = _nickname;
	std::cout << "old nickname= " << old_nick << std::endl;
	this->_nickname = _arg_registration.back();
	std::cout << "new nickname= " << _nickname << std::endl;
	if (!NicknameIsValid())
	{
		setMessage(reply(ERR_ERRONEUSNICKNAME, this));
		this->_flag_erroneus = true;
		if (isAuthenticate())
			_nickname = old_nick;
		return;
	}
	if (checkSameNick() == true)
	{
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
	std::string self_rpl;
	std::vector<Client *> saveclient;

	// récupere le parametre apres les ´:´ (reason param)
	size_t pos = 0;
	for (size_t i = 0; i != _arg_registration.size(); i++)
	{
		if (_arg_registration[i].find_first_of(':', 0) != std::string::npos)
			pos = i;
	}
	res = _arg_registration[pos];
	if (_arg_registration.size() > 1)
	{
		pos++;
		for (; pos != _arg_registration.size(); pos++)
		{
			res += " " + _arg_registration[pos];
		}
	}
	quit_reason = res;

	self_rpl += "ERROR: Server closing a client connection\r\n";
	self_rpl += ":" + get_nickname() + "!" + get_user() + "@" + get_hostname() + " QUIT :" + "Quit " + quit_reason + "\r\n";
	setMessage(self_rpl);
	broadcast_rpl += ":" + get_nickname() + "!" + get_user() + "@" + get_hostname() + " QUIT :" + "Quit " + quit_reason + "\r\n";
	std::vector<Channel *>::iterator it_chan;
	for (it_chan = this->_channels->begin(); it_chan != _channels->end(); it_chan++)
	{
		if ((*it_chan)->has_clients(this))
		{
			std::vector<Client *> vectclients = (*it_chan)->getClients();
			std::vector<Client *>::iterator it_client;
			for (it_client = vectclients.begin(); it_client != vectclients.end(); it_client++)
			{
				if (*it_client != this && !hasalready(*it_client, saveclient)) // do not send the message channels times to this
				{
					saveclient.push_back(*it_client);
					(*it_client)->setMessage(broadcast_rpl);
				}
			}
			((*it_chan)->deleteClientFromChan(this));
			(this)->deleteOperator(*it_chan);
			if ((*it_chan)->getClients().size() < 1)
				(*it_chan)->set_flag_erase_chan(true);
			else
				(*it_chan)->search_new_ope(this);
		}
	}
	this->_flag_shut_client = true;
}

void Client::cap(const std::string &) {} // obligee de prendre en compte cette commande pour ne pas interrompre le traitement des cmd

bool Client::checkParams(std::string const &password)
{
	unsigned int i = 0;
	std::string rpl;

	void (Client::*func_list[])(std::string const &arg) =
		{&Client::cap, &Client::checkPassword, &Client::Nick, &Client::checkUser,
		 &Client::clean_ping_mode, &Client::quit};
	std::string cmd_to_check[] = {"CAP", "PASS", "NICK", "USER", "PING", "QUIT"};
	while (i < (sizeof(func_list) / sizeof(*func_list))) // calcul le nombre d'element d'un tableau present SUR LA STACK (only)
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
			}
			else
			{
				int before_step = this->_step_registration;
				(this->*(func_list[i]))(password);
				if (before_step != this->_step_registration)
					this->authenticationValid();
			}
			return true;
		}
		i++;
	}
	return false;
}

void Client::Clean_arg()
{
	std::vector<std::string>::iterator it = _arg_registration.begin();
	while (it != _arg_registration.end())
		_arg_registration.erase(it);
}

bool Client::getCmdLine(/*std::string const &password*/)
{
	const std::string eol_marker = "\r\n";

	size_t pos;
	std::string cmd_line;

	pos = this->_message_recv.find(eol_marker);
	if (pos == std::string::npos)
		return false;

	cmd_line = _message_recv.substr(0, pos);
	set_arg();
	tokenization_cmd(cmd_line);
	_message_recv.erase(_message_recv.begin(), (_message_recv.begin() + pos + eol_marker.length()));
	return true;
}

bool Client::hasalready(Client *client, std::vector<Client *> saveclient)
{
	std::vector<Client *>::iterator it_client;
	for (it_client = saveclient.begin(); it_client != saveclient.end(); it_client++) // ici si remplace _clients par getClients())
	{
		if ((*it_client)->getSocketClient() == client->getSocketClient())
			return true;
	}
	return false;
}

void Client::broadcaster(std::string const &reply)
{
	std::vector<Client *> saveclient;

	std::vector<Channel *>::iterator it_chan;
	for (it_chan = this->_channels->begin(); it_chan != _channels->end(); it_chan++)
	{
		if ((*it_chan)->has_clients(this))
		{
			std::vector<Client *> vectclients = (*it_chan)->getClients();
			std::vector<Client *>::iterator it_client;
			for (it_client = vectclients.begin(); it_client != vectclients.end(); it_client++)
			{
				if (*it_client != this && !hasalready(*it_client, saveclient)) // do not send the message channels times to this
				{
					(*it_client)->setMessage(reply);
					saveclient.push_back(*it_client);
				}
			}
		}
	}
	setMessage(reply);
}

bool Client::isAuthenticate()
{
	if (_pass_ok == true && _nick_ok == true && _user_ok == true)
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