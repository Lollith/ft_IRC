#include "irc.h"

Channel::Channel ( std::string name ): _name(name){
	// std::cout << "constructeur channel par default"<< std::endl;
}

std::string Channel::getName( void ) const{
	return this->_name;

}

std::vector<Client *> Channel::getClients( void ) const {
	return(this->_clients);
}

void Channel::addClient(Client *client){
	INFO("client"<< client->getSocketClient()<<" rejoint" << std::endl);
	this->_clients.push_back(client);
	// std::cout << "vector client"<< _clients.back()->getSocketClient()<< std::endl;
}



//check si le client est dans le  vector
bool Channel::hasClient(Client *client)
{
	std::vector<Client*>::iterator it_client;
	for (it_client = _clients.begin(); it_client != _clients.end(); it_client++)// ici si remplace _clients par getClients())
	{
		if ((*it_client)->getSocketClient() == client->getSocketClient())
			return true;
	}
	return false;
}

void Channel::deleteClient(Client *client)
{

}