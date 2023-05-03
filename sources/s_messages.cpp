#include "irc.hpp"

// The PRIVMSG command is used to send private messages between users, as well 
// as to send messages to channels. <target> is the nickname of a client or the name of a channel.(#)

void Server::privmsg( Client *client){
	int size = client->get_arg().size() - 2;
	std::string target = client->get_arg()[size];
	std::string msg = client->get_arg().back();
	std::string priv_notice = " PRIVMSG ";

	if(_flag_notice == true)
		priv_notice = " NOTICE ";

	if (target[0] == '#')
		privmsg_to_chan(client, priv_notice, target, msg);
	else
		privmg_to_client(client, priv_notice, target, msg);
}

// The difference between NOTICE and PRIVMSG is that automatic replies must never
//be sent in response to a NOTICE message.
void Server::notice( Client *client)
{
	_flag_notice = true;
	privmsg(client);
}

//recherche parmi mon vector de channels , le bon channel , puis envoyer le 
//message aux bons client = clients enregistres dans le channel
void Server::privmsg_to_chan(Client *client, std::string &priv_notice, std::string &target, std::string &msg)
{
	std::vector<Channel*>::iterator it_chan;	
	for (it_chan = _channels.begin(); it_chan != _channels.end(); it_chan++)
	{
		if ((*it_chan)->getName() == target)
		{
			std::string message = ":" + client->get_nickname() + priv_notice + target + " " + msg + "\r\n";
			size_t i = 0;
			while (i!= (*it_chan)->getClients().size()) //broadcast the messag
			{
				if ((*it_chan)->getClients()[i] != client) // remplace le set chaine vide 
					(*it_chan)->getClients()[i]->setMessage(message);
				i++;
			}
			_flag_notice = false;
			return;
		}
		else
		{
			if(_flag_notice == false)
				client->setMessage(reply(ERR_NOSUCHCHANNEL, client, target));
			_flag_notice = false;
			return;
		}
	}
}


// na pas trouver le bon channel : check les pseudo pour envoyer a un nickname
void Server::privmg_to_client(Client *client, std::string &priv_notice, std::string &target,std::string &msg)
{
	std::cout << priv_notice << std::endl;
	std::vector<Client*>::iterator it_client;	
	for (it_client = _client.begin(); it_client != _client.end(); it_client++)
	{
		if ((*it_client)->get_nickname() == target)
		{
			std::string message = ":" + client->get_nickname() + priv_notice + (*it_client)->get_nickname() + " " + msg + "\r\n";
			(*it_client)->setMessage(message);
			_flag_notice = false;
			return;
		}
	}
	if(_flag_notice == false)
		client->setMessage(reply(ERR_NOSUCHNICK, client, target));
	_flag_notice = false;
	return;
}