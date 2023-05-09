#include "irc.hpp"
Channel::~Channel(){}

Channel::Channel( std::string name ): _name(name){
_flag_erase_chan = false;
_topic = "welcome";
	_mode[I]= "-";
	_mode[S]= "-";
	_mode[T]= "-";
}

std::string Channel::getName( void ) const{
return this->_name;
}

std::string Channel::getTopic( void ) const{
return this->_topic;
}

void	Channel::setTopic( std::string newtopic ){
_topic = newtopic;
}

time_t Channel::get_topic_time( void ) const{
	return this->_topic_time;
}

void Channel::set_topic_time( time_t timestamp )
{
	_topic_time = timestamp;
}

time_t Channel::get_create_time( void ) const{
	return this->_topic_time;
}

void Channel::set_create_time( time_t timestamp )
{
	_create_time = timestamp;
}

std::vector<Client *> Channel::getClients( void ) const {
return(this->_clients);
}

std::string	*Channel::get_mode( void )
{
	return _mode;
}

void Channel::set_mode(std::string mode)
{
	if (mode == "+i" || mode == "-i")
		_mode[I] = mode[0]; // recup le + ou -
	if (mode == "+s" || mode == "-s")
		_mode[S] = mode[0];
	if (mode == "+t" || mode == "-t")
		_mode[T] = mode[0];
}

bool	Channel::get_flag_erase_chan( void ) const{
	return(_flag_erase_chan);
}

void Channel::set_flag_erase_chan( bool mybool ){
_flag_erase_chan = mybool;
}

bool Channel::is_invited(Client *client)
{
	if(client->get_nickname() == this->_invited)
		return true;
	return false;
}

void Channel::set_invite(Client *newclient)
{
	_invited = newclient->get_nickname();
}

void Channel::addClient(Client *client){
	INFO("client"<< client->getSocketClient()<<" rejoint" << std::endl);
	this->_clients.push_back(client);
}


//check si le client est dans le  vector
bool Channel::has_clients(Client *client)
{
	std::vector<Client*>::iterator it_client;
	for (it_client = _clients.begin(); it_client != _clients.end(); it_client++)// ici si remplace _clients par getClients())
	{
		if ((*it_client)->getSocketClient() == client->getSocketClient())
			return true;
	}
		return false;
}

bool Channel::has_clients(std::string newclient)
{
	std::vector<Client*>::iterator it_client;
	for (it_client = _clients.begin(); it_client != _clients.end(); it_client++)// ici si remplace _clients par getClients())
	{
		if ((*it_client)->get_nickname() == newclient)
			return true;
	}
		return false;
}


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

void Channel::search_new_ope(Client *client)
{
	if(has_clients(client))
		client->add_chan_ope(this);
}

void Channel::check_vctor(Client *client)
{
	(void) client;
	DEBUG("vector _client in chan "<< this->getName()<<" :");
	std::vector<Client*>::iterator it_client;
	for (it_client = _clients.begin(); it_client != _clients.end(); it_client++)// ici si remplace _clients par getClients())
		DEBUG((*it_client)->get_nickname() << " ");
	std::cout <<std::endl;
// 	std::vector<Channel*>::iterator it2_chan;
// 	for (it2_chan = client->get_chan_ope().begin(); it2_chan != client->get_chan_ope().end(); it2_chan++)// ici si remplace _clients par getClients())
// 		DEBUG (client->get_nickname()<<" is operator of chan "<< (*it2_chan)->getName()<< std::endl);
}
