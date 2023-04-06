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

void Client::tokenization_cmd(std::string cmd)
{
	size_t pos;
	size_t i = 0;

	// pos = this->_message_recv.find(' ', i);
	// if (pos == std::string::npos)
	// 	; // ici mettre une logique correspondante
	// // std::string str_tmp;
	// this->_cmd_registration = _message_recv.substr(i, (pos - i));
	// //debugg__________________________________________________________________
	// std::cout << "i : " << i << std::endl;
	// std::cout << "in tokenization: cmd is : " << this->_cmd_registration << std::endl;
	// //_____end____________________________________________________________________
	// i = pos + 1;
	// //debugg___________________________________________________________________
	// std::cout << "pos : " << pos << std::endl;
	// std::cout << "i after pos + 1: " << i << std::endl;
	// //__end_______________________________________________________________________
	// pos = this->_message_recv.find(' ', i);
	// this->_arg_registration.push_back(_message_recv.substr(i, (pos - i)));
	// //debugg___________________________________________________________________
	// std::cout << "pos : " << pos << std::endl;
	// std::cout << "in tokenization: args are : " << this->_arg_registration.back()<< std::endl;
	// //__end_______________________________________________________________________
	// _message_recv.erase(_message_recv.begin(), _message_recv.begin()+ pos);
	// //debugg___________________________________________________________________
	// std::cout << "in tokenization: what is after erase : " << this->_arg_registration.front() << std::endl;
	// //__end_______________________________________________________________________
}

void Client::getCmdLine()
{
	size_t i = 0;
	size_t pos;
	std::string cmd_line;
	pos = this->_message_recv.find("/r/n", i);
	while( i != std::string::npos)
	{
		cmd_line = _message_recv.substr(i, (pos - i));
		tokenization_cmd(cmd_line);
		checkParams();
		_message_recv.erase(_message_recv.begin(),  _message_recv.begin() + pos);
		pos = this->_message_recv.find("/r/n", i);
	}
}