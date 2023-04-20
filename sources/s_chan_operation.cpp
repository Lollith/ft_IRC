#include "irc.h"
//-----fct _channels------------------------------------------------------------
void Server::parse_msg_recv(Client *client, std::string msg_recv)
{
	int nb_fct = 4;
	std::string funct_names[] = {"JOIN", "QUIT", "PRIVMSG", "NAMES"};

	void (Server::*fct_member[])(Client *client, std::string arg) = { &Server::join, &Server::quit, &Server::privmsg, &Server::names};

	for (int i = 0; i < nb_fct; i++)
	{
		if (msg_recv.find(funct_names[i]) != std::string::npos)
		{
			(this->*fct_member[i])(client, msg_recv);
		}
	}

}




void Server::join( Client *client, std::string arg )
{
	(void) arg;
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
	welcome_new_chan(client, _channels.back());	


}

//A JOIN message with the client as the message <source> and the channel they 
//have joined as the first parameter of the message.
// The <source> of the message represents the user or server that sent the message, 
// and the <target> represents the target of that PRIVMSG (which may be the client, 
// a channel, etc).

// The channel’s topic (with RPL_TOPIC (332) and optionally RPL_TOPICWHOTIME (333)), 
//and no message if the channel does not have a topic.
// A list of users currently joined to the channel (with one or more RPL_NAMREPLY 
//(353) numerics followed by a single RPL_ENDOFNAMES (366) numeric). These 
//RPL_NAMREPLY messages sent by the server MUST include the requesting client that has just joined the channel.
void Server::welcome_new_chan(Client *client, Channel *channel)
{
	std::string join_msg = ":"+ client->get_user() + "@" +"~" + client->get_hostname() + " JOIN "+ _channels.back()->getName() +"\r\n";
	join_msg += reply(RPL_TOPIC, client, channel);
	join_msg += reply(RPL_NAMREPLY, client, channel);
	join_msg += reply(RPL_ENDOFNAMES, client, channel);
	client->setMessage(join_msg);
}



void Server::quit(Client *client, std::string arg)
{
	(void) client;
	(void ) arg;
	std::cout << "=>Quit le channel" << std::endl;
}

//______________________________TEST CTRLC
void Server::stop()
{
	this->_flag_keep_loop = false;
}

// The PRIVMSG command is used to send private messages between users, as well 
// as to send messages to channels. <target> is the nickname of a client or the name of a channel.(#)

void Server::privmsg( Client *client, std::string arg ){
	(void) arg;
	int size = client->get_arg().size() - 2;
	std::string target = client->get_arg()[size];
	std::string msg = client->get_arg().back();
	INFO("je recois les messages prives depuis le client " + client->get_user()+ "\n");
	// check si commence par un # => chan
	if (target[0] == '#')
	{
	//recherche parmi mon vector de channels , le bon channel , puis envoyer le message aux bons client = clients enregistres dans le channel
		std::vector<Channel*>::iterator it_chan;	
		for (it_chan = _channels.begin(); it_chan != _channels.end(); it_chan++)
		{
			if ((*it_chan)->getName() == target)
			{
				std::string message = ":" + client->get_user() + " PRIVMSG " + target + " " + msg + "\r\n";
				size_t i = 0;
				while (i!= (*it_chan)->_clients.size()) //broadcast the messag
				{
					(*it_chan)->_clients[i]->setMessage(message);
					i++;
				}
					client->setMessage("");// interdit le client en cours de recevoir son propre message 
			}
		}
	}
	// na pas trouver le bon channel : check les pseudo pour envoyer a un nickname
	std::vector<Client*>::iterator it_client;	
	for (it_client = _client.begin(); it_client != _client.end(); it_client++)
	{
		if ((*it_client)->get_nickname() == target)
		{
			std::string message = ":" + client->get_user() + " PRIVMSG " + (*it_client)->get_nickname() + " " + msg + "\r\n";
			(*it_client)->setMessage(message);
		}
	}
		client->get_arg().erase(client->get_arg().end()-1);
}

void Server::names(Client *client, std::string arg){ // a faire ????
(void) arg;
(void) client;
	// INFO("execute la fct names\n");
}