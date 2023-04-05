#ifndef MESSAGE_HPP
#define MESSAGE_HPP
#include <string>
#include <map>


class Message{
	private: 
		std::string _buffer;
		std::string _cmd;
		std::map<size_t, std::string> _param;

		Message( void );

	public:
		Message(std::string &buffer);
		std::string getCmd(void) const;
		std::map<size_t, std::string> getParams(void)  const;

};


class Connection : public Message
{

};

class Disconnection : public Message
{

};

class UserData : public Message
{

};

#endif