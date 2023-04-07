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


//__________________________________________________MEMBERS

