#include "irc.h"

Channel::Channel ( std::string name ): _name(name){
	std::cout << "constructeur channel par default"<< std::endl;
}

std::string Channel::getName( void ) const{
	return this->_name;

}

void Channel::addClient(){
	std::cout << "client rejoint"<< std::endl;
}