#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "irc.hpp"

// message appartiennent a la classe client
// a faire une fonction par commande
class Message
{

	//__________________________________________________canonic form
public:
	Message(void);
	Message(std::string &buffer);
	Message(Message const &copy);
	~Message(void);
	Message &operator=(Message const &rhs);

	// std::string const& getCmd(void) const;
	// std::vector<std::string> const& getParams(void)  const;

	//__________________________________________________GETTERS_SETTERS
	std::string getBuffer(void) const;
	void setBuffer(std::string buffer);
	void resetBuffer();

	// attributes________________________________________________________________
protected:
	std::string _buffer;
	// std::string _cmd;
	// std::vector<std::string> const& getParams(void)  const;

	// std::map<size_t, std::string> _param;
};

class Connection : public Message
{
public:
	Connection(void);
	Connection(std::string &buffer);
	Connection(Connection const &cpy);
	~Connection(void);
	Connection &operator=(Connection const &rhs);
	// heritage ; cap ID
	// check si cap    => std::cout << num erreur,...
};

// class Disconnection : public Message
// {

// };

// class UserData : public Message
// {

// };

#endif