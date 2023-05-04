#include "irc.hpp"
//-----fct _channels------------------------------------------------------------
void Server::parse_msg_recv(Client *client, std::string msg_recv)
{
	int nb_fct = 4;
	std::string funct_names[] = {"JOIN", "PART", "TOPIC", "PRIVMSG", "NOTICE"};

	void (Server::*fct_member[])(Client *client) = { 
		&Server::join, 
		&Server::part,
		&Server::topic,
		&Server::privmsg, 
		&Server::notice};

	for (int i = 0; i < nb_fct; i++)
	{
		if (msg_recv.find(funct_names[i]) != std::string::npos)
		{
			(this->*fct_member[i])(client);
			client->set_arg();
			// client->setMsgRecvSave(""); // reinitialise le message recu sinon boucle sur /quit
		}
	}
}

void Server::broadcast_all(Client *client, Channel *chan, std:: string msg)
{
	std::string nickname = client->get_nickname();
	for (size_t i = 0; i!= chan->getClients().size(); i++)
	{
		// if (chan->getClients()[i] != client)
			chan->getClients()[i]->setMessage(msg);
	}
	return;

}

Channel *Server::searchChan(std::string name)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i]->getName() == name)
			return _channels[i];
	}
	return (NULL);
}

void Server::join( Client *client)
{
	size_t i = 0;

	while(i < client->get_arg().size())
	{
		Channel *chan = searchChan(client->get_arg()[i]);
		if (!chan)
		{
			chan = new Channel( client->get_arg()[i]);
			_channels.push_back(chan); // si chan n existe pas => le creer
			INFO("creation Channel " + client->get_arg()[i] + "\n");
			client->add_chan_ope(chan);
		}
			INFO("=>Join le channel\n");
			(chan)->addClient(client);
			welcome_new_chan(client, chan);
			chan->check_vctor(client);
		i++;
	}
}

//A JOIN message with the client as the message <source> and the channel they 
//have joined as the first parameter of the message.
// The <source> of the message represents the user or server that sent the message, 
// and the <target> represents the target of that PRIVMSG (which may be the client, 
// a channel, etc).

void Server::welcome_new_chan(Client *client, Channel *channel)
{
	std::string nickname = client->get_nickname();
	std::string join_msg_all = ":"+ nickname + "@" + client->get_hostname() + " JOIN " + _channels.back()->getName() + "\r\n";
	broadcast_all(client, channel, join_msg_all);
	
	std::string join_msg;
	if(channel->getTopic() == "")
		join_msg += reply(RPL_NOTOPIC, client, channel->getName());
	else
		join_msg += reply(RPL_TOPIC, client, channel);
	join_msg += reply(RPL_NAMREPLY, client, channel);
	join_msg += reply(RPL_ENDOFNAMES, client, channel->getName());
	client->setMessage(join_msg);
}

 Channel *Server::has_chan(Client * client)
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

void Server::part(Client *client)
{	
	std::string msg = "";
	//TODO part tous les chans de la list ?
	std::string chan_arg = client->get_arg().at(0);
	
	if (client->get_arg().size() == 2)
		msg = client->get_arg().back();
	
	Channel *chan = has_chan(client); // check si chan existe
	if(chan!= NULL)
	{
		INFO("=>leave le channel" << std::endl);
		if(chan->has_clients(client))
		{
			std::string message =  ":" + client->get_nickname()+ "@" + client->get_hostname() + " PART " + chan_arg + " " + msg + "\r\n";
			broadcast_all(client, chan, message);
			chan->deleteClientFromChan(client);
			client->deleteOperator(chan);
			if(chan->getClients().size() < 1)
				chan->set_flag_erase_chan(true);
			else
				chan->search_new_ope(client);
			chan->check_vctor(client);
			return;
		}
		else
		{
			client->setMessage(reply(ERR_NOTONCHANNEL, client, chan_arg));
			return;
		}
	}
	else 
	{
		client->setMessage(reply(ERR_NOSUCHCHANNEL, client, chan_arg));
		return;
	}
}

void Server::topic(Client *client)
{
	std::string msg;
	Channel *chan = has_chan(client);
	std::string chan_arg = client->get_arg().at(0);

//TODO : parcourir la liste de chan donner comme pour join
	if (chan) // arg[0]
	{
		if(!chan->has_clients(client))
		{
			client->setMessage(reply(ERR_NOTONCHANNEL, client, chan_arg));
			return;
		}
		if(!client->is_operator(chan))
		{
			client->setMessage(reply(ERR_CHANOPRIVSNEEDED, client, chan->getName()));
			return;
		}
	// TODO ?	
		// if (client->get_arg().size() < 1)
		// {
		// 	std::cout << "here " << std::endl;
		// 	client->setMessage(reply(ERR_NEEDMOREPARAMS, client, chan->getName()));
		// 	return;  
		// }
		if (client->get_arg().size() == 2)
		{
			chan->setTopic(client->get_arg().at(1));
			msg = reply(RPL_TOPIC, client, chan);
		}
		else if (client->get_arg().size() == 1)	
		{
				chan->setTopic( "" );
				msg = reply(RPL_NOTOPIC, client, chan->getName());
		}
		chan->set_topic_time(std::time(NULL));
		msg += reply(RPL_TOPICWHOTIME, client, chan);
		std::cout << msg << std::endl;
		broadcast_all(client, chan, msg);
	}
	else 
	{
		client->setMessage(reply(ERR_NOSUCHCHANNEL, client, chan_arg));
		return;
	}
}


// void Server::names(Client *client){ // TODO
// (void) client;
// 	// INFO("execute la fct names\n");
// }

//TODO
// list message
//invite message: operator
//kick : operator

//______________________________TEST CTRLC
void Server::stop()
{
	this->_flag_keep_loop = false;
}

