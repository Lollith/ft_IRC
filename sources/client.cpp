#include "client.hpp"
#include "server.hpp"

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

void Client::setMsgRecv( std::string buf){
	this->_message_recv = buf;
}

void Client::tokenization_cmd(std::string& cmd)
{
	size_t pos;
	size_t i = 0;

	//verif contenu cmd line ex: CAP LS \r\n
	
	std::cout << "complete cmd line in tokenisation is : " << cmd << std::endl;

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

void Client::getCmdLine()
{
	const std::string eol_marker = "\r\n";
	size_t pos;
	std::string cmd_line;

	pos = this->_message_recv.find(eol_marker);
	while (pos != std::string::npos)
	{
		cmd_line = _message_recv.substr(0, pos);
		tokenization_cmd(cmd_line);
		// checkParams();
		_message_recv.erase(_message_recv.begin(), (_message_recv.begin() + pos + eol_marker.length()));
		pos = this->_message_recv.find(eol_marker);
	}
}