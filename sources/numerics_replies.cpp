#include "irc.hpp"

std::string reply (int reply, Client *client, Channel *channel)
{
	std::string msg;
	std::string name;
	
	switch(reply)
	{
		case RPL_LIST:
			msg = "322 " + client->get_nickname() + " " + channel->getName() + " " +ft_itoa(channel->getClients().size()) + " :" + channel->getTopic() +"\r\n";
			break;

		case RPL_TOPIC:
			msg = "332 " + client->get_nickname() + " " + channel->getName() + " " + channel->getTopic() +"\r\n";
			break;
		
		case RPL_TOPICWHOTIME:
			msg = "333 " + client->get_nickname() + " " + channel->getName() + " " + client->get_nickname() + " " +ft_itoa(channel->get_topic_time()) + "\r\n";
			break;
		

		case RPL_NAMREPLY:
			for (size_t  i = 0; i < channel->getClients().size(); i++)
			{
					if (channel->getClients()[i]->get_mode() == "+i" && !channel->has_clients(client))
						name += " ";
					else
						name += channel->getClients()[i]->get_nickname()+ " ";
			}
			if (name != " ")
				msg = "353 " + client->get_nickname() + " = " + channel->getName() + " :@" + name + "\r\n";
			else
				msg = "353 " + client->get_nickname() + " = " + channel->getName() + "\r\n";
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
		case RPL_LISTSTART:
			msg = "321 " + client->get_nickname() + " Channel :Users Name \r\n";
			break;
		
		case RPL_LISTEND:
			msg = "323 " +client->get_nickname() + " :End of /LIST\r\n";
			break;
		
		case RPL_NOTOPIC:
			msg = "331 " + client->get_nickname() + " " + target + " :No topic is set\r\n";
			break;
		
		case  RPL_INVITING:
			msg = "341 " + client->get_nickname() + " " + client->get_arg()[0] + " " + target + "\r\n";
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
		
		case ERR_USERONCHANNEL:
			msg = "443 " + client->get_nickname() + " " + client->get_arg()[0] + " " + target + " :is already on channel\r\n";
			break;

		case ERR_NEEDMOREPARAMS:
			msg = "461 " + client->get_nickname() + " " + client->get_cmd() + " :Not enough parameters\r\n";
			break;

		case ERR_CHANOPRIVSNEEDED:
			msg = "482 " + client->get_nickname() + " " + target + " :You're not channel operator\r\n";
			break;

		default:
			msg = "erreur";   // a redefinir
	}
	return (msg);
}

std::string reply (int reply, Client *client, std::string target, std::string newnick)
{
	std::string msg;

	switch(reply)
	{
		case ERR_USERNOTINCHANNEL:
			msg = "441 " + client->get_nickname() + " " + newnick + " " + target + " :They aren't on that channel\r\n";
			break;
		
		// case ERR_USERONCHANNEL: //TODO ici
			// msg = "443 " + client->get_nickname() + " " + client->get_arg()[0] + " " + target + " :is already on channel\r\n";
			// break;
		
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