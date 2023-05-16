#include "irc.hpp"

void Server::broadcast_all(Client *client, Channel *chan, std:: string &msg)
{
	std::string nickname = client->get_nickname();
	for (size_t i = 0; i!= chan->getClients().size(); i++)
		chan->getClients()[i]->setMessage(msg);
	return;
}

Channel *Server::searchChan(std::string &name)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i]->getName() == name)
			return _channels[i];
	}
	return (NULL);
}


Client *Server::searchClient(std::string &name)
{
	for (size_t i = 0; i < _client.size(); i++)
	{
		if (_client[i]->get_nickname() == name)
			return _client[i];
	}
	return (NULL);
}

//mal ecrit => search _cahn retourn un chan, has_chan retourn un bool
 Channel *Server::has_chan(Client *client)
{
	std::string chan = client->get_arg().at(0);
	std::vector<Channel*>::iterator it_chan;	
	for (it_chan = this->_channels.begin(); it_chan != _channels.end(); it_chan++)
	{
		if ((*it_chan)->getName() == chan)
			return (*it_chan);
	}
	return (NULL);
}

Channel* Server::check_chan(Client *client, std::string &chan_arg)
{
	Channel *chan = searchChan(chan_arg); // check si chan existe
	if(!chan)
	{
		client->setMessage(reply(ERR_NOSUCHCHANNEL, client, chan_arg));
		return NULL;
	}
	if(!chan->has_clients(client))
	{
		client->setMessage(reply(ERR_NOTONCHANNEL, client, chan->getName()));
		return NULL;
	}
	return chan;
}

std::string Server::append_user_message(Client *client)
{
	std::string msg;

	for (size_t i = 1; i < client->get_arg().size(); i++)
	{
		msg += client->get_arg()[i];
		if (i != client->get_arg().size() - 1)
			msg += " ";
	}
	msg += "\r\n";
	return (msg);
}

//______________________________TEST CTRLC
void Server::stop()
{
	this->_flag_keep_loop = false;
}
