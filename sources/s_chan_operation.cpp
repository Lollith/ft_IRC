#include "irc.hpp"
//-----fct _channels------------------------------------------------------------
void Server::parse_msg_recv(Client *client, std::string msg_recv)
{
	int nb_fct = 10;
	std::string funct_names[] = {
		"JOIN", 
		"PART", 
		"TOPIC", 
		"PRIVMSG", 
		"NOTICE", 
		"NAMES", 
		"MODE", 
		"LIST", 
		"INVITE",
		"KICK"};

	void (Server::*fct_member[])(Client *client) = { 
		&Server::join, 
		&Server::part,
		&Server::topic,
		&Server::privmsg, 
		&Server::notice,
		&Server::names,
		&Server::mode,
		&Server::list,
		&Server::invite,
		&Server::kick};

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


Client*Server::searchClient(std::string name)
{
	for (size_t i = 0; i < _client.size(); i++)
	{
		if (_client[i]->get_nickname() == name)
			return _client[i];
	}
	return (NULL);
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


void Server::mode(Client *client)
{	
	std::string message;
	std::string target = client->get_arg()[0];
	std::string msg = "-i";

	if (client->get_arg().size() == 2)
		msg = client->get_arg()[1];

	if (target[0] == '#')
		message += ":" + client->get_nickname()+ "@" + client->get_hostname() + " MODE " + target + " " + msg + "\r\n";
	else
		message +=  ":" + client->get_nickname()+ "@" + client->get_hostname() + " MODE " + target + " " + msg + "\r\n";
	client->setMessage(message);
}



	//TODO name tous les chans de la list 
	// The NAMES command is used to view the nicknames joined to a channel and their 
	// channel membership prefixes. The param of this command is a list of channel names, 
	// delimited by a comma (",", 0x2C) character . pb tokenisation]
	//FIXME pas de tokenisation?
void Server::names(Client *client){ // TODO
// (void) client;
	// si  1 arg
	Channel *chan_arg = has_chan(client);
	std::string msg;

	// for(size_t i = 0; i < chan_list.size(); i++)
	// {
	// 	std::cout << chan_list[i]<< " "<< std::endl;
	// }
	
	if(chan_arg)
	{
		msg += reply(RPL_NAMREPLY, client, chan_arg);
		// msg += reply(RPL_ENDOFNAMES, client, chan_arg->getName());
		// client->setMessage(msg);
	}
	else
	{
		msg += (reply(ERR_NOSUCHCHANNEL, client,client->get_arg()[0]));
	}
		msg += reply(RPL_ENDOFNAMES, client, client->get_arg()[0]);
		client->setMessage(msg);


	// // //liste arg
	// std::vector<std::string> chan_list; 
	// for(size_t i = 0; i < client->get_arg().size() - 1; i++)
	// {
	// 	chan_list = split(client->get_arg()[i], ",");
	// 	std::cout << chan_list[i] << std::endl;
	// }
// 
	// = split(has_chan(client)->getName(),",");
	
	return;

}

void Server::list(Client *client)
{
	std::string msg;

	if (client->get_arg()[0]== "")  //si == 0 ne rentre pas ds la fonction => pareil pour Name => pas de lecture de requette
	{
		msg += reply(RPL_LISTSTART, client, "");
		for (size_t i = 0; i < _channels.size(); i++)
		{
			msg += reply(RPL_LIST, client, _channels[i]);
		}
		msg += reply(RPL_LISTEND, client, "");
	}
	else
	{
		std::vector<std::string> chan_list; 
		msg += reply(RPL_LISTSTART, client, "");
		chan_list = split(client->get_arg()[0], ",");
		for (size_t i = 0; i < chan_list.size(); i++)
		{
			Channel *chan = searchChan(chan_list[i]);
			if (chan)
				msg += reply(RPL_LIST, client, chan);
		}
		msg += reply(RPL_LISTEND, client, "");
	}
		client->setMessage(msg);
	return;
}
//invite message: operator

void Server::invite(Client *client)
{
	if (client->get_arg().size() < 2)
	{
		client->setMessage(reply(ERR_NEEDMOREPARAMS, client, ""));
		return;
	}
	Channel *chan = searchChan(client->get_arg()[1]); // check si chan existe
	if(chan!= NULL)
	{
		if(chan->has_clients(client))
		{
			if (!client->is_operator(chan))
			{
				client->setMessage(reply(ERR_CHANOPRIVSNEEDED , client, chan->getName()));
				return;
			}
			Client *new_target = searchClient(client->get_arg()[0]);
			if (!new_target)
			{
				client->setMessage(reply(ERR_NOSUCHNICK, client, chan->getName() ));
				return;
			}
			if (chan->has_clients(client->get_arg()[0]))
			{
				client->setMessage(reply(ERR_USERONCHANNEL, client, chan->getName()));
				return;
			}

			std::string msg = reply( RPL_INVITING, client, chan->getName());
			client->setMessage(msg);
			std::string invite_msg = ":"+ new_target->get_nickname() + "@" + new_target->get_hostname() + " INVITE " + new_target->get_nickname()+ " " +chan->getName() + "\r\n";
			new_target->setMessage(invite_msg);
			return;
		}
		else
		{
			client->setMessage(reply(ERR_NOTONCHANNEL, client, chan->getName()));
			return;
		}
	}
	else 
	{
		client->setMessage(reply(ERR_NOSUCHCHANNEL, client, client->get_arg()[1]));
		return;
	}

}

//Kick <channel> <user>(","<user>)(comment)
void Server::kick(Client *client)
{
	if (client->get_arg().size() < 2)
	{
		client->setMessage(reply(ERR_NEEDMOREPARAMS, client, ""));
		return;
	}
	
	Channel *chan = searchChan(client->get_arg()[0]); // check si chan existe
	if(chan)
	{
		if(chan->has_clients(client))
		{
			if (!client->is_operator(chan))
			{
				client->setMessage(reply(ERR_CHANOPRIVSNEEDED , client, chan->getName()));
				return;
			}

			std::vector<std::string> client_list; 
			client_list = split(client->get_arg()[1], ",");
			for (size_t i = 0; i < client_list.size(); i++)
			{
				Client *new_target = searchClient(client_list[i]);


				if (!new_target)
				{
					client->setMessage(reply(ERR_USERNOTINCHANNEL, client, chan->getName(), client->get_arg()[1] ));
					return;
				}

				std::string comment = "kicked\r\n";
				if (client->get_arg().size() == 3)
					comment = client->get_arg().back();
				std::string kick_msg = ":"+ client->get_nickname() + "@" + client->get_hostname() + " KICK " + chan->getName() + " " +new_target->get_nickname()+ " :" + comment + "\r\n";
				new_target->setMessage(kick_msg);
				chan->deleteClientFromChan(new_target);
				broadcast_all(client, chan, kick_msg);
				return;
			}
		}
		else
		{
				client->setMessage(reply(ERR_NOTONCHANNEL, client, chan->getName()));
				return;
		}
	}
	else 
	{
		client->setMessage(reply(ERR_NOSUCHCHANNEL, client, client->get_arg()[0]));
		return;
	}
}

//______________________________TEST CTRLC
void Server::stop()
{
	this->_flag_keep_loop = false;
}

