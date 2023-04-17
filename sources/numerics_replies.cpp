#include "irc.h"

std::string reply (int reply, Client *client, Channel *channel)
{
	std::string msg;

	switch(reply)
	{
		case RPL_TOPIC:
			msg = "332 " + client->get_user() + " " + channel->getName() + " :welcome\r\n"; // tester plusiuers chan , join sur  le 1er
			break;
		case RPL_NAMREPLY:
			msg = "353=" + channel->getName() + " :" + client->get_nickname() + "\r\n"; // utilise la NAME commande a faire
			break;
		case RPL_ENDOFNAMES:
			msg = "366 " + channel->getName() +" :End of /NAMES list \r\n";  //NAME A FAIRE
			break;
		default:
			msg = "erreur";   // a redefinir
	}
	return (msg);
}