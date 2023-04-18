#include "irc.h"

Channel::Channel ( std::string name ): _name(name){
	// std::cout << "constructeur channel par default"<< std::endl;
}

std::string Channel::getName( void ) const{
	return this->_name;

}

void Channel::addClient(Client *client){
	std::cout << "client"<< client->getSocketClient()<<" rejoint" << std::endl;
	this->_clients.push_back(client);
	// std::cout << "vector client"<< _clients.back()->getSocketClient()<< std::endl;

}