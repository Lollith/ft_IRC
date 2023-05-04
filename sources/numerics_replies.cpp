#include "irc.hpp"

std::string reply (int reply, Client *client, Channel *channel)
{
	std::string msg;
	std::string name;
	
	switch(reply)
	{
		case RPL_TOPIC:
			msg = "332 " + client->get_nickname() + " " + channel->getName() + " " + channel->getTopic() +"\r\n";
			break;
		
		case RPL_TOPICWHOTIME:
			msg = "333 " + client->get_nickname() + " " + channel->getName() + " " + client->get_nickname() + " " +ft_itoa(channel->get_topic_time()) + "\r\n";
			break;
		

		case RPL_NAMREPLY:
			for (size_t  j = 0; j < channel->getClients().size(); j++)
				name += channel->getClients()[j]->get_nickname()+ " ";
			msg = "353 " + client->get_nickname() + " = " + channel->getName() + " :@" + name + "\r\n";
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
		
		case RPL_NOTOPIC:
			msg = "331 " + client->get_nickname() + " " + target + " :No topic is set\r\n";
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

		// case ERR_NEEDMOREPARAMS:
		// 	msg = "461 " + client->get_nickname() + " " + client->get_cmd() + " :Not enough parameters\r\n";
		// 	break;

		case ERR_CHANOPRIVSNEEDED:
			msg = "482 " + client->get_nickname() + " " + target + " :You're not channel operator\r\n";
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
			msg = "431 " + client->get_nickname() +  " :Nickname non given\r\n";
			break;
		case ERR_NICKCOLLISION  :
			msg = "436 " + client->get_nickname() +  " :Nickname collision\r\n";
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

std::string ft_itoa(int n)
{
	std::string result;
	std::stringstream _out;
	
	_out << n;
	result = _out.str();
	return result;
}


std::vector<std::string> split(const std::string &str, const char *set_delim)
{
	std::size_t i = 0;
	std::vector<std::string> list;

	std::size_t pos = str.find_first_of(set_delim);

	while (pos != std::string::npos)
	{
		if (pos == i)
		{
			i = ++pos;
		}
		else
		{
			list.push_back(str.substr(i, pos - i));
			i = ++pos;
		}
		pos = str.find_first_of(set_delim, pos);
	}

	if (i != str.length())
		list.push_back(str.substr(i, str.length()));

	return list;
}