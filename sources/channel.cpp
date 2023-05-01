#include "irc.h"

Channel::Channel ( std::string name ): _name(name){
	_flag_erase_chan = false;
	_topic = " :welcome";
	// std::cout << "constructeur channel par default"<< std::endl;
}

std::string Channel::getName( void ) const{
	return this->_name;

}

std::vector<Client *> Channel::getClients( void ) const {
	return(this->_clients);
}

bool	Channel::get_flag_erase_chan( void ) const{
	return(_flag_erase_chan);
}

void Channel::set_flag_erase_chan( bool mybool ){
	_flag_erase_chan = mybool;
}


void Channel::addClient(Client *client){
	INFO("client"<< client->getSocketClient()<<" rejoint" << std::endl);
	this->_clients.push_back(client);
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

// TODO: adeline
void Channel::deleteClientFromChan(Client *client)
{

	std::vector<Client*>::iterator it = this->_clients.begin();
	while( it != _clients.end())
	{
		if ((*it)->getSocketClient() == client->getSocketClient())
			_clients.erase(it);
		else
			it++;
	}

}

