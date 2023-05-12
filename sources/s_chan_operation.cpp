#include "irc.hpp"

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
		}
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
	std::string join_msg_all = ":"+ nickname + "@" + client->get_hostname() + 
		" JOIN " + channel->getName() + "\r\n";
	broadcast_all(client, channel, join_msg_all);
	
	std::string join_msg;
	if(channel->getTopic() == "")
		join_msg += reply(RPL_NOTOPIC, client, channel->getName());
	else
		join_msg += reply(RPL_TOPIC, client, channel);
	channel->set_topic_time(std::time(NULL));
	join_msg += reply(RPL_NAMREPLY, client, channel);
	join_msg += reply(RPL_ENDOFNAMES, client, channel->getName());
	client->setMessage(join_msg);
}

///JOIN #test,#test2
void Server::join( Client *client )
{
	if (client->get_arg().size() < 1)
		return(client->setMessage(reply(ERR_NEEDMOREPARAMS, client, "")));
	std::vector<std::string> chan_list;
	chan_list = split(client->get_arg()[0], ",");
	for (size_t i = 0; i < chan_list.size(); i++)
	{
		if (chan_list[i][0] != '#')
			chan_list[i] =  "#" + chan_list[i]; 
		
		Channel *chan = searchChan(chan_list[i]);
		if (!chan)
		{
			chan = new Channel( chan_list[i]);
			chan->set_create_time(std::time(NULL));
			_channels.push_back(chan);
			INFO("creation Channel " + chan_list[i] + "\n");
			client->add_chan_ope(chan);
		}
		if ( chan->get_mode()[I] == "-" || chan->is_invited(client))
		{
			INFO("=>Join le channel\n");
			(chan)->addClient(client);
			welcome_new_chan(client, chan);
			chan->check_vctor(client);
		}
		else
			return(client->setMessage(reply(ERR_INVITEONLYCHAN, client, chan->getName())));
	}
}

void Server::part(Client *client)
{
	std::string msg = "";
	
	if (client->get_arg().size() < 1)
		return(client->setMessage(reply(ERR_NEEDMOREPARAMS, client, "")));
	
	if (client->get_arg().size() == 2)
		msg = client->get_arg().back();
	std::vector<std::string> chan_list; 
	chan_list = split(client->get_arg()[0], ",");
	for (size_t i = 0; i < chan_list.size(); i++)
	{
		Channel *chan = searchChan(chan_list[i]);
		if(!chan)
			return(client->setMessage(reply(ERR_NOSUCHCHANNEL, client, chan_list[i])));
		if(!chan->has_clients(client))
			return(client->setMessage(reply(ERR_NOTONCHANNEL, client, chan_list[i])));
		
		INFO("=>leave le channel" << std::endl);
		std::string message =  ":" + client->get_nickname()+ "@" + 
		client->get_hostname() + " PART " + chan_list[i] + " " + msg + "\r\n";
		broadcast_all(client, chan, message);
		chan->deleteClientFromChan(client);
		client->deleteOperator(chan);
		if(chan->getClients().size() < 1)
			chan->set_flag_erase_chan(true);
		else
			chan->search_new_ope(client);
		chan->check_vctor(client);
	}
	return;
}

// +t: If this mode is enabled, users must have channel privileges such as halfop 
// or operator status in order to change the topic of a channel. In a channel 
// that does not have this mode enabled, anyone may set the topic of the channel 
// using the TOPIC command.
void Server::topic(Client *client)
{
	if (client->get_arg().size() < 1)
		return(client->setMessage(reply(ERR_NEEDMOREPARAMS, client, "")));
	
	std::string msg;
	Channel *chan = has_chan(client);
	std::string chan_arg = client->get_arg().at(0);
	if (!chan) // arg[0]
		return(client->setMessage(reply(ERR_NOSUCHCHANNEL, client, chan_arg)));
	if ((chan->get_mode()[T] == "+" && client->is_operator(chan)) || chan->get_mode()[T] == "-" )
	{
		if(!chan->has_clients(client))
			return(client->setMessage(reply(ERR_NOTONCHANNEL, client, chan_arg)));
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
		broadcast_all(client, chan, msg);
	}
	else
		return(client->setMessage(reply(ERR_CHANOPRIVSNEEDED, client, chan->getName())));
}

// /mode target [<+i>]
void Server::mode(Client *client)
{	
	std::string target = client->get_arg()[0];
	std::string mode = client->get_mode();

	if (client->get_arg().size() == 2)
	{ 
		if (!(client->get_arg()[1][0] == '+' || client->get_arg()[1][0] == '-' )) // ctrl apparition de lettres
			return;
	}
	if (client->get_arg().size() == 2 && 
		(client->get_arg()[1][0] == '+' || client->get_arg()[1][0] == '-' )) // ctrl apparition de lettres
			mode = client->get_arg()[1];
	if (target[0] == '#')
		chan_mode(client, target, mode);
	else
		user_mode(client, target, mode);
}

void Server::chan_mode(Client *client, std::string &target, std::string &mode)
{
	std::string message;
	Channel *chan = client->search_chan(target);
	if( !chan)
	{
		return(client->setMessage(reply(ERR_NOSUCHCHANNEL, client, target)));
	}
	if (client->get_arg().size() < 2)
	{
		std::string rep = (reply (RPL_CHANNELMODEIS, client, chan));	
		rep += reply ( RPL_CREATIONTIME, client, chan);	
		client->setMessage(rep);
		return;
	}
	if (client->is_operator(chan))
	{
		chan->set_mode(mode);
		message += ":" + client->get_nickname()+ "@" + client->get_hostname() + 
		" MODE " + target + " " + mode + "\r\n";
		broadcast_all(client, chan, message);
	}
	else
		return(client->setMessage(reply(ERR_CHANOPRIVSNEEDED , client, chan->getName())));
}

void Server::user_mode(Client *client, std::string &target, std::string &mode)
{
	std::string message;
	if (!searchClient(target))
		return(client->setMessage(reply(ERR_NOSUCHNICK, client, target )));
	if ( client->get_nickname() != target )
		return(client->setMessage(reply(ERR_USERSDONTMATCH, client)));
	if (client->get_arg().size() < 2)
		return(client->setMessage(reply (RPL_UMODEIS, client)));	
	std::cout << mode << std::endl;	
	if (mode == "+i" || mode == "-i")
	{
		client->set_mode(mode);
		message +=  ":" + client->get_nickname()+ "@" + client->get_hostname() + 
			" MODE " + target + " " + mode + "\r\n";
		client->setMessage(message);
	}
	else
		client->setMessage(reply (ERR_UMODEUNKNOWNFLAG, client));	
}

	// The NAMES command is used to view the nicknames joined to a channel and their 
	// channel membership prefixes. The param of this command is a list of channel names, 
	// delimited by a comma (",", 0x2C) character . pb tokenisation]
	// /names <channel>,<channel> 
	// chan mode +s, user mode +i
void Server::names(Client *client){
	
	std::string msg;

	std::vector<std::string> chan_list; 
	chan_list = split(client->get_arg()[0], ",");
	for (size_t i = 0; i < chan_list.size(); i++)
	{
		Channel *chan = searchChan(chan_list[i]);
		if (!chan || (chan->get_mode()[S] == "+" && !chan->has_clients(client)))
			msg += reply(RPL_ENDOFNAMES, client, chan_list[i]);
		else 
		{
			msg += reply(RPL_NAMREPLY, client, chan);
			msg += reply(RPL_ENDOFNAMES, client, chan_list[i]);
		}
	}
	client->setMessage(msg);
	return;
}

void Server::list(Client *client)
{
	std::string msg;

	if (client->get_arg()[0] == "")
	{
		msg += reply(RPL_LISTSTART, client, "");
		for (size_t i = 0; i < _channels.size(); i++)
		{
			if (_channels[i]->get_mode()[S] == "-" || _channels[i]->has_clients(client))
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

 // / invite nickanem channel
void Server::invite(Client *client)
{
	if (client->get_arg().size() < 2)
		return(client->setMessage(reply(ERR_NEEDMOREPARAMS, client, "")));
	
	Channel *chan = check_chan(client, client->get_arg()[1]); // check si chan existe
	if (!chan)
		return;
	if (!client->is_operator(chan) && chan->get_mode()[I] == "+")
		return(client->setMessage(reply(ERR_CHANOPRIVSNEEDED, client, chan->getName())));
	
	Client *new_target = searchClient(client->get_arg()[0]);
	if (!new_target)
		return(client->setMessage(reply(ERR_NOSUCHNICK, client, chan->getName())));
	if (chan->has_clients(client->get_arg()[0]))
		return(client->setMessage(reply(ERR_USERONCHANNEL, client, chan->getName())));
	
	std::string msg = reply( RPL_INVITING, client, chan->getName());
	client->setMessage(msg);
	chan->set_invite(new_target);
	std::string invite_msg = ":"+ client->get_nickname() + "@" + 
		client->get_hostname() + " INVITE " + new_target->get_nickname()+ 
		" " + chan->getName() + "\r\n";
	new_target->setMessage(invite_msg);
	return;
}

//Kick <channel> <user>(","<user>)(comment)
void Server::kick(Client *client)
{
	if (client->get_arg().size() < 2)
		return(client->setMessage(reply(ERR_NEEDMOREPARAMS, client, "")));

	Channel *chan = check_chan(client, client->get_arg()[0]); // check si chan existe
	if (!chan)
		return;
	if (!client->is_operator(chan))
		return(client->setMessage(reply(ERR_CHANOPRIVSNEEDED , client, chan->getName())));

	std::vector<std::string> client_list; 
	client_list = split(client->get_arg()[1], ",");
	for (size_t i = 0; i < client_list.size(); i++)
	{
		Client *new_target = chan->searchClient(client_list[i]);
		if (!new_target)
			return(client->setMessage(reply(ERR_USERNOTINCHANNEL, client, 
				chan->getName(), client->get_arg()[1])));

		if (client == new_target)
			return;
		
		std::string comment = "kicked\r\n";
		if (client->get_arg().size() > 2)
			comment = append_user_message(client);
		std::string kick_msg = ":"+ client->get_nickname() + "@" + 
			client->get_hostname() + " KICK " + chan->getName() + " " + 
			new_target->get_nickname()+ " :" + comment + "\r\n";
		new_target->setMessage(kick_msg);
		chan->deleteClientFromChan(new_target);
		broadcast_all(client, chan, kick_msg);
		return;
	}
}


