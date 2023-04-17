#include "irc.h"
//-----fct _channels------------------------------------------------------------
void Server::parse_msg_recv(Client *client, std::string msg_recv)
{
	int nb_fct = 3;
	std::string funct_names[] = {"JOIN", "QUIT", "PRIVMSG"};

	void (Server::*fct_member[])(Client *client, std::string arg) = { &Server::join, &Server::quit, &Server::privmsg };

	for (int i = 0; i < nb_fct; i++)
	{
		if (msg_recv.find(funct_names[i]) != std::string::npos)
			(this->*fct_member[i])(client, msg_recv);
	}
}






void Server::join( Client *client, std::string arg )
{
	std::string channel = client->get_arg().back();	
	// Topic set by X[] [time]
	std::vector<Channel*>::iterator it;	
	for (it = _channels.begin(); it != _channels.end(); it++) // 1er n existe pas , ne rentre pas
	{
		if ((*it)->getName() == channel) // si  channel existe
		{
			INFO("=>Join le channel\n");
			(*it)->addClient(client);
			welcome_new_chan(client);
			return;
		}
	}
	_channels.push_back(new Channel( channel)); // si chan n existe pas => le creer
	INFO("creation Channel " + _channels.back()->getName()+ "\n");
	_channels.back()->addClient(client);
	welcome_new_chan(client);


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
void Server::welcome_new_chan(Client *client)
{
	std::string join_msg = ":"+ client->get_user() + "@" +"~" + client->get_hostname() + " JOIN "+ _channels.back()->getName() +"\r\n";
	join_msg += reply(RPL_TOPIC, client, _channels);
	join_msg += reply(RPL_NAMREPLY, client, _channels);
	join_msg += reply(RPL_ENDOFNAMES, client, _channels);
	client->setMessage(join_msg);
}



void Server::quit(Client *client, std::string arg)
{
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
int size = client->get_arg().size() - 2;
	std::cout << "je recois les messages prives depuis le client"<< client->getSocketClient()<< std::endl;
	
	//recherche parmi mon vector de channels , le bon channel , puis envoyer le message aux bons client = clients enregistres dans le channel
	std::vector<Channel*>::iterator it;	
	for (it = _channels.begin(); it != _channels.end(); it++)
	{
		if ((*it)->getName() == client->get_arg()[size])
		{
			std::string l(":lollith");
			std::string p(" PRIVMSG ");
			// std::cout<< "message recu: "<<client->get_arg().back()<< ",a envoyer a:"<< client->get_arg()[size]<< std::endl;
			std::string message =  l + p + client->get_arg()[size] + " " +client->get_arg().back() + "\r\n";
			int i = 0;
			while (i!= (*it)->_clients.size())
			{
				(*it)->_clients[i]->setMessage(message);
				i++;
			}
				client->setMessage("");// interdit le client en cours de recevoir son propre message 
		}
	}
}