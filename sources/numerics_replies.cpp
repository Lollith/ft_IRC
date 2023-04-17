#include "irc.h"

std::string reply (int reply, Client *client, std::vector<Channel*> _channels)
{
	std::string msg;

	switch(reply)
	{
		case RPL_TOPIC:
			msg = "332 " + client->get_user() + " " + _channels.back()->getName() + " :welcome\r\n"; // attention pas forcerment le dernier du channel
			break;
		case RPL_NAMREPLY:
			msg = "353=" + _channels.back()->getName() + " :" + client->get_nickname() + "\r\n"; // utilise la NAME commande a faire
			break;
		case RPL_ENDOFNAMES:
			msg = "366 " + _channels.back()->getName() +" :End of /NAMES list \r\n";  //NAME A FAIRE
			break;
		default:
			msg = "erreur";   // a redefinir
	}
	return (msg);
}