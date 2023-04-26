#include "irc.h"

std::string reply (int reply, Client *client, Channel *channel)
{
	std::string msg;
	std::string name;
	
	switch(reply)
	{
		case RPL_NAMREPLY:
			for (size_t  j = 0; j < channel->getClients().size(); j++)
				name += channel->getClients()[j]->get_nickname()+ " ";
			msg = "353 " + client->get_nickname() + " = " + channel->getName() + " :" + name + "\r\n";
			break;
		default:
			msg = "erreur";   // a redefinir
	}
	return (msg);
}

std::string reply (int reply, Client *client, std::string target)
{
	std::string msg;

	switch(reply)
	{
		case RPL_TOPIC:
			msg = "332 " + client->get_nickname() + " " + target + " :welcome\r\n";
			break;
		
		case RPL_ENDOFNAMES:
			msg = "366 "+ client->get_nickname() + " " + target + " :End of /NAMES list\r\n";
			break;
		
		case ERR_NOSUCHNICK:
			msg = "401 " + client->get_nickname() + " " + target + " :No such nick\r\n";
			break;

		case ERR_NOSUCHCHANNEL:
			msg = "403 " + client->get_nickname() + " " + target + " :No such channel\r\n";
			break;
		
		case ERR_NOTONCHANNEL:
			msg = "442 " + client->get_nickname() + " " + target + " :You're not on that channel\r\n";
			break;

		default:
			msg = "erreur";   // a redefinir
	}
	return (msg);
}

std::string reply (int reply, Client *client)
{
	std::string msg;

	switch(reply)
	{
		case ERR_ERRONEUSNICKNAME:
			msg = "432 " + client->get_nickname() + " " + client->get_nickname() + " :Erroneus nickname\r\n";
			break;
		case ERR_NICKNAMEINUSE:
			msg = "433 " + client->get_nickname() + " " + client->get_nickname() +  " :Nickname is already in use\r\n";
			break;
		case ERR_NONICKNAMEGIVEN :
			msg = "431 " + client->get_nickname() + " " + client->get_nickname() +  " :Nickname non given\r\n";
			break;
		case ERR_NICKCOLLISION  :
			msg = "436 " + client->get_nickname() + " " + client->get_nickname() +  " :Nickname collision\r\n";
			break;
		case ERR_NOTREGISTERED:
			msg = "451 " + client->get_nickname() + " :You have not registered\r\n";
			break;
		case ERR_NEEDMOREPARAMS:
			msg = "461 " + client->get_nickname() + " :Not enough parameters\r\n";
			break;
		case ERR_ALREADYREGISTERED:
			msg = "462 " + client->get_nickname() + " :You may not reregister\r\n";
			break;
		case ERR_PASSWDMISMATCH:
			msg = "464 " + client->get_nickname() + " :Password incorrect\r\n";
			break;
		default:
			msg = "erreur";   // a redefinir
	}
	return (msg);
}