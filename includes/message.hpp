#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "irc.hpp"

// message appartiennent a la classe client
class Message
{
	//__________________________________________________canonic form
public:
	Message(void);
	Message(Message const &copy);
	~Message(void);
	Message &operator=(Message const &rhs);
	//__________________________________________________GETTERS_SETTERS
	std::string getBuffer(void) const;
	void setBuffer(std::string buffer);
	void resetBuffer();

	// attributes________________________________________________________________
protected:
	std::string _buffer;


};


#endif