#include "irc.h"
//-----fct _channels------------------------------------------------------------
void Server::parse_msg_recv(Client *client, std::string msg_recv)
{
	int nb_fct = 4;
	std::string funct_names[] = {"JOIN", "QUIT", "PRIVMSG", "NAMES"};

	void (Server::*fct_member[])(Client *client) = { &Server::join, &Server::quit, &Server::privmsg, 
		&Server::names};

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

void Server::welcome_new_chan(Client *client, Channel *channel)
{
	// erreur ici : reinitialise si 2 meme nom???? a tester avec 2 pseudo differents
	// std::string join_msg2 = ":" + client->get_user() + "@" + "~" + client->get_hostname() + " JOIN " + _channels.back()->getName() + "\r\n";
	// for (size_t i = 0; i!= channel->_clients.size(); i++) //broadcast the message :nouveau client joigned aux autres du chan
	// 	channel->_clients[i]->setMessage(join_msg2);
	
	std::string join_msg = ":" + client->get_nickname() + "@" + "~" + client->get_hostname() + " JOIN " + _channels.back()->getName() + "\r\n";
	
	join_msg += reply(RPL_TOPIC, client, channel->getName());
	join_msg += reply(RPL_NAMREPLY, client, channel);
	join_msg += reply(RPL_ENDOFNAMES, client, channel->getName());
	client->setMessage(join_msg);
}

void Server::quit(Client *client)
{	
	std::string msg = client->get_arg().back();
	std::string message = ":" + client->get_nickname()+ "@" + "~" +client->get_hostname()+ " QUIT " +  msg + "\r\n";

	std::vector<Channel*>::iterator it_chan;	
	for (it_chan = this->_channels.begin(); it_chan != _channels.end(); it_chan++)
	{
		if((*it_chan)->hasClient(client))
		{
			std::vector<Client*> vectclients = (*it_chan)->getClients();
			std::vector<Client*>::iterator it_client;	
			for (it_client = vectclients.begin(); it_client != vectclients.end(); it_client++) 
				(*it_client)->setMessage(message);
		}
		INFO("=>Quit le channel" << std::endl);
		client->setMessage("");// interdit le client en cours de recevoir son propre message 
	}
}


// The PRIVMSG command is used to send private messages between users, as well 
// as to send messages to channels. <target> is the nickname of a client or the name of a channel.(#)

void Server::privmsg( Client *client){
	int size = client->get_arg().size() - 2;
	std::string target = client->get_arg()[size];
	std::string msg = client->get_arg().back();
	// INFO("je recois les messages prives depuis le client " + client->get_user()+ "\n");
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
				while (i!= (*it_chan)->getClients().size()) //broadcast the messag
				{
					if ((*it_chan)->getClients()[i] != client) // remplace le set chaine vide 
						(*it_chan)->getClients()[i]->setMessage(message);
					i++;
				}
				// client->setMessage("");// interdit le client en cours de recevoir son propre message 
			}
			else
				client->setMessage(reply(ERR_NOSUCHCHANNEL, client, target));
		}
	}
	else
	{
	// na pas trouver le bon channel : check les pseudo pour envoyer a un nickname
		std::vector<Client*>::iterator it_client;	
		for (it_client = _client.begin(); it_client != _client.end(); it_client++)
		{
			if ((*it_client)->get_nickname() == target)
			{
				std::string message = ":" + client->get_user() + " PRIVMSG " + (*it_client)->get_nickname() + " " + msg + "\r\n";
				(*it_client)->setMessage(message);
			}
			else
				client->setMessage(reply(ERR_NOSUCHNICK, client, target));
		}
	}
}

void Server::names(Client *client){ // a faire ????
(void) client;
	// INFO("execute la fct names\n");
}

//______________________________TEST CTRLC
void Server::stop()
{
	this->_flag_keep_loop = false;
}