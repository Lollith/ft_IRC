
#include "irc.hpp"

void Client::deleteOperator(Channel *chan)
{
	std::vector<Channel *>::iterator it = this->_chan_ope.begin();
	while( it != _chan_ope.end())
	{
		if ((*it)->getName() == chan->getName())
			_chan_ope.erase(it);
		else
			it++;
	}
}

bool Client::is_operator(Channel *chan)
{
	if(this->getSocketClient() == chan->getClients().at(0)->getSocketClient())
			return true;
	return false;
}

std::vector<Channel*> Client::get_chan_ope(void) const
{
	return this->_chan_ope;
}
	
void Client::add_chan_ope( Channel *chan )
{
	_chan_ope.push_back(chan);
}

Channel* Client::search_chan(std::string chan)
{
	std::vector<Channel*>::iterator it;
	for (it = _channels->begin(); it != _channels->end(); it++)
	{
		if ((*it)->getName() == chan)
			return (*it);
	}
		return NULL;
}