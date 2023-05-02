#include "irc.h"
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

void Server::broadcast(Client *client, Channel *chan)
{
	std::string nickname = client->get_nickname();
	std::string join_msg_all = ":"+ nickname + "@" + client->get_hostname() + " JOIN " + _channels.back()->getName() + "\r\n";
	for (size_t i = 0; i!= chan->getClients().size(); i++)
	{
		// if (chan->getClients()[i] != client)
			chan->getClients()[i]->setMessage(join_msg_all);
	}
	return;

}

void Server::join( Client *client)
{
	//TODO : tabl de channel pour recup la liste des arg => plusieurs channels
	std::vector<std::string > list_channel;
	size_t i = 0;
	while(i < client->get_arg().size())
	{
		// std::cout << client->get_arg()[i]<< std::endl;
		list_channel.push_back(client->get_arg()[i]);
		i++;
	} 
	

	std::vector<Channel*>::iterator it;	
	for (it = _channels.begin(); it != _channels.end(); it++) // 1er n existe pas , ne rentre pas
	{
		i = 0;
		while(i < list_channel.size())
		{
			if ((*it)->getName() == list_channel[i]) // si  channel existe
			{
				INFO("=>Join le channel\n");
				(*it)->addClient(client);
				welcome_new_chan(client, *it);
			}
			i++;
		}
		//TODO return ici => separer la fct
	}
	// for(size_t i = 0; i != 10; i++)
	i = 0;
	while(i < list_channel.size())
	{
		_channels.push_back(new Channel( list_channel[i])); // si chan n existe pas => le creer
		INFO("creation Channel " + list_channel[i] + "\n");
		_channels.back()->addClient(client);
		client->_chan_ope = true;
		welcome_new_chan(client, _channels.back());
		i++;
	}
		return;
}

//A JOIN message with the client as the message <source> and the channel they 
//have joined as the first parameter of the message.
// The <source> of the message represents the user or server that sent the message, 
// and the <target> represents the target of that PRIVMSG (which may be the client, 
// a channel, etc).

void Server::welcome_new_chan(Client *client, Channel *channel)
{
//broadcast the message :nouveau client joigned aux autres du chan
	std::string nickname = client->get_nickname();
	broadcast(client, channel);
	
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
	//TODO part tous les chans de la list
	std::string chan_arg = client->get_arg().at(0);
	
	if (client->get_arg().size() == 2)
		msg = client->get_arg().back();
	
	Channel *chan = has_chan(client);
	if(chan!= NULL)
	{
		INFO("=>leave le channel" << std::endl);
		if(chan->hasClient(client))
		{
			std::string message =  ":" + client->get_nickname()+ "@" + client->get_hostname() + " PART " + chan_arg + " " + msg + "\r\n";
			std::vector<Client*> vectclients = chan->getClients();
			std::vector<Client*>::iterator it_client;				
			for (it_client = vectclients.begin(); it_client != vectclients.end(); it_client++)
					(*it_client)->setMessage(message);
			chan->deleteClientFromChan(client);
			client->_chan_ope = false;
			//TODO chercher le client n 0 de la liste => le passer en operator
			if(chan->getClients().size() < 1)
				chan->set_flag_erase_chan(true);
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

//si operator ://TODO
void Server::topic(Client *client)
{
	if(client->_chan_ope == false)
	{
		// ERR_CHANOPRIVSNEEDED (482) //TODO
		return;
	}
	Channel *chan = has_chan(client);
	std::string msg;
//TODO : parcourir la liste de chan donner comme pour join
	if (chan)
	{
		chan->setTopic( "" );
		msg = reply(RPL_NOTOPIC, client, chan->getName());

		if (client->get_arg().size() == 2)
		{
			chan->setTopic(client->get_arg().at(1));
			msg = reply(RPL_TOPIC, client, chan);
		}
		client->setMessage((msg));
	}
}//TODO
// ERR_NOSUCHCHANNEL (403)
// ERR_NOTONCHANNEL (442)
// RPL_TOPICWHOTIME (333)
//envoyer a tous les clients le new topic memem vide


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



//TODO
// operator chan? avec ~
	//std::string message =  ":" + client->get_nickname() + "@" + "~" + client->get_hostname() +  " PART " + chan + " " + msg + "\r\n";