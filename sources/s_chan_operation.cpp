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
			client->setMsgRecvSave(""); // reinitialise le message recu sinon boucle sur /quit
		}
	}
}

void Server::join( Client *client)
{
	//TODO : tabl de channel pour recup la liste des arg => plusieurs channels
	std::string channel = client->get_arg().back();	
	std::vector<Channel*>::iterator it;	
	for (it = _channels.begin(); it != _channels.end(); it++) // 1er n existe pas , ne rentre pas
	{
		if ((*it)->getName() == channel) // si  channel existe
		{
			INFO("=>Join le channel\n");
			(*it)->addClient(client);
			welcome_new_chan(client, *it);
			return;
		}
	}
	_channels.push_back(new Channel( channel)); // si chan n existe pas => le creer
	INFO("creation Channel " + channel + "\n");
	_channels.back()->addClient(client);
	 client->_chan_ope = true;
	welcome_new_chan(client, _channels.back());	
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
		
	std::string join_msg = ":"+ nickname + "@" + client->get_hostname() + " JOIN " + _channels.back()->getName() + "\r\n";
	for (size_t i = 0; i!= channel->getClients().size(); i++)
		channel->getClients()[i]->setMessage(join_msg);
		// std::cout << channel->getClients()[i]->get_nickname()<<std::endl;
	if(channel->_topic == "")
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
	Channel *chan = has_chan(client);
	std::string msg;
//TODO : parcourir la liste de chan donner comme pour join
	if (chan)
	{
		chan->_topic = "";
		msg = reply(RPL_NOTOPIC, client, chan->getName());

		if (client->get_arg().size() == 2)
		{
			chan->_topic = client->get_arg().at(1);
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