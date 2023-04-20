#include "irc.h"

std::string reply (int reply, Client *client, Channel *channel)
{
	std::string msg;
	std::string name;

	switch(reply)
	{
		case RPL_TOPIC:
			msg = "332 " + client->get_user() + " " + channel->getName() + " :welcome\r\n";
			break;
		case RPL_NAMREPLY:
			for (size_t  j = 0; j < channel->_clients.size(); j++)
				name += channel->_clients[j]->get_nickname()+ " "; 
			msg = "353 " + client->get_user()+" = " + channel->getName() + " :" + name + "\r\n";
			break;
		case RPL_ENDOFNAMES:
			msg = "366 "+ client->get_user()+ " " + channel->getName() +" :End of /NAMES list\r\n";
			break;
		default:
			msg = "erreur";   // a redefinir
	}
	return (msg);
}