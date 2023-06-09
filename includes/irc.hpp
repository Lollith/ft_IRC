#ifndef IRC_HPP
# define IRC_HPP

#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <signal.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> //htons +define the in_addr cf <netinet/in.h>
#include <set>
#include <vector>
#include <ctime>
#include <sstream>
#include "message.hpp"
#include "client.hpp"
#include "channel.hpp"
#include "server.hpp"
class Channel;
class Client;

#define RESET_TXT "\e[0m"

#define RED_TXT "\e[31m"
#define GREEN_TXT "\e[32m"
#define YELLOW_TXT "\e[33m"
#define BLUE_TXT "\e[34m"
#define MAG_TXT "\e[35m"
#define CYAN_TXT "\e[36m"
#define ITALIC_TXT "\e[2m"

#define DEBUG(x) std::cerr << BLUE_TXT << x << RESET_TXT;
#define FATAL_ERR(x) std::cerr << RED_TXT << x << RESET_TXT;
#define INFO(x) std::cerr << YELLOW_TXT << x << RESET_TXT;
#define RUNTIME_MSG(x) std::cerr << GREEN_TXT << x << RESET_TXT;

enum reply_code
{
	RPL_WELCOME = 001,
	RPL_UMODEIS = 221,
	RPL_WHOISUSER = 311,
	RPL_ENDOFWHO = 315,
	RPL_ENDOFWHOIS = 318,
	RPL_WHOISCHANNELS = 319,
	RPL_LISTSTART = 321,
	RPL_LIST = 322,
	RPL_LISTEND = 323,
	RPL_CHANNELMODEIS = 324,
	RPL_CREATIONTIME = 329,
	RPL_NOTOPIC = 331,
	RPL_TOPIC = 332,
	RPL_TOPICWHOTIME = 333,
	RPL_INVITING = 341,
	RPL_WHOREPLY = 352,
	RPL_NAMREPLY = 353,
	RPL_ENDOFNAMES = 366,
	RPL_YOUREOPER = 381,
	ERR_NOSUCHNICK = 401,
	ERR_NOSUCHCHANNEL = 403,
	ERR_UNKNOWNCOMMAND = 421,
	ERR_NONICKNAMEGIVEN = 431,
	ERR_ERRONEUSNICKNAME = 432,
	ERR_NICKNAMEINUSE = 433,
	ERR_NICKCOLLISION = 436,
	ERR_USERNOTINCHANNEL = 441,
	ERR_NOTONCHANNEL = 442,
	ERR_USERONCHANNEL = 443,
	ERR_NOTREGISTERED = 451,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTERED = 462,
	ERR_PASSWDMISMATCH = 464,
	ERR_UNKNOWNMODE = 472,
	ERR_INVITEONLYCHAN = 473,
	ERR_BADCHANMASK = 476,
	ERR_NOPRIVILEGES = 481,
	ERR_CHANOPRIVSNEEDED = 482,
	ERR_UMODEUNKNOWNFLAG = 501,	
	ERR_USERSDONTMATCH = 502
};

enum modes
{
	I = 0,
	S = 1,
	O = 1,
	T = 2
};

std::string reply (int reply, Client *client);
std::string reply (int reply, Client *client, std::string target);
std::string reply (int reply, Client *client, Channel *channel);
std::string reply (int reply, Client *client, std::string target, std::string newnick);
std::string ft_itoa(int n);
std::vector<std::string> split(const std::string &str, const char *set_delim);

#endif