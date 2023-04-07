#include "irc.h"

//__________________________________________________canonic form

Client::Client(void){
	// std::cout << "constructeur client par default"<< std::endl;
}

// Client::Client(int sock_client): _socket_client(sock_client)
// {
// 	std::cout << "create client" << std::endl;
// }



// Client::Client(Client const &cpy)
// {
// }

// Client &Client::operator=(Client const &rhs)
// {
// }

Client::~Client(void) {} // close() ou/et freeinfo() à faire?

//__________________________________________________GETTERS_SETTERS


int Client::getSocketClient() const
{
	return this->_socket_client;
}

void Client::setSocketClient(int socket)
{
	this->_socket_client = socket;
}

std::string	Client::getMessage( void ) const{
	return this->_message.getBuffer();
}

void Client::setMessage( std::string buffer ){
	this->_message.setBuffer(buffer);
}

void Client::setMsgRecv( std::string buf){
	_message_recv = buf;
}

std::string Client::getMsgRecv( void ) const{
	return this->_message_recv;
}

void Client::tokenization_cmd(std::string& cmd)
{
	size_t pos;
	size_t i = 0;

	//verif contenu cmd line ex: CAP LS \r\n
	
	// std::cout << "complete cmd line in tokenisation is : " << cmd << std::endl;

	pos = cmd.find(' ', i);
	this->_cmd_registration = cmd.substr(i, (pos - i));
	while (pos != std::string::npos)
	{	
		i = pos + 1;
		pos = cmd.find(' ', i);
		this->_arg_registration.push_back(cmd.substr(i, (pos - i)));

	}
	//vérifs attributs cmd and args
	std::cout << "in tokenization: cmd is : " << this->_cmd_registration << std::endl;
	std::cout << "in tokenization: args are : " << this->_arg_registration.back()<< std::endl;
}


void Client::ignoreCap(std::string const&)
{
	std::cout << "here is CAP check func" << std::endl;

	this->_step_registration += 1;
}

void Client::checkPassword(std::string const& psswd)
{
	std::cout << "here is PASS check func" << std::endl;
	
	if (_arg_registration.back() == psswd)
	{
		this->_step_registration += 1;
		this->_flag_password_ok = true;
	}
	else if (_flag_password_ok == true)
	{
		//ici construct Connexion buffer to send response ERR_ALREADYREGISTERED //462
		
		shutdown(_socket_client, SHUT_RDWR);
		//close();
		//free_something_i_dont_know_what;
		return ;
	}
	else if ((_cmd_registration == "PASS") && (_arg_registration.empty()))
	{
		//ici construct Connexion buffer to send response ERR_NEEDMOREPARAMS //462
		
		shutdown(_socket_client, SHUT_RDWR);
		//close();
		//free_something_i_dont_know_what;
		return ;
	}
	else
	{
		//ici construct Connexion buffer to send response ERR_PASSWDMISMATCH //462
		
		shutdown(_socket_client, SHUT_RDWR);
		//close();
		//free_something_i_dont_know_what;
		return ;
	}
}

void Client::checkNick(std::string const&)
{
	std::cout << "here is NICK check func" << std::endl;
	
	this->_step_registration += 1;

}

void Client::checkUser(std::string const&)
{
	std::cout << "here is USER check func" << std::endl;
	
	this->_step_registration += 1;


}



void Client::checkParams(std::string const& password)
{
	int i = 0;
	void (Client::*func_list[4])(std::string const& arg) = 
		{&Client::ignoreCap, &Client::checkPassword, &Client::checkNick, &Client::checkUser};
	std::string	cmd_to_check[4] = {"CAP", "PASS", "NICK", "USER"};
	while (i < 4)
	{
		//QQPART CHECKER L EXISTANCE DE PASS DANS LES CMD RECV
		// -> CHECK FLAG_PASSWORD_OK 
		// (dans le if add un if i == 1 != PASS && flag == false return error ??)
		if (_cmd_registration == cmd_to_check[i])
		{
			(this->*(func_list[i]))(password);
			break;
		}
		i++;
	}
}

void Client::getCmdLine(std::string const& password)
{
	const std::string eol_marker = "\r\n"; // à mettre ds un define?
	this->_flag_password_ok = false;
	size_t pos;
	std::string cmd_line;

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