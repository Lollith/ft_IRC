#include "irc.h"

//__________________________________________________canonic form
Message::Message( void ):_buffer( "001 lollith :welcome to the localhost Network, lollith! lollith@localhost\r\n"){
	//   "<client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]"
}

Message::Message( std::string &buffer ): _buffer(buffer){
}


Message::Message( Message const &copy ){
	*this = copy;
}

Message::~Message( void ) { 
}

Message &Message::operator=( Message const &rhs )
{
	if (this != &rhs)
	{
		this->_buffer = rhs._buffer;
	}
	return (*this);
}

//__________________________________________________GETTERS_SETTERS
std::string Message::getBuffer( void )const{
	return(_buffer);
}

void 	Message::setBuffer( std::string buffer ){
	this->_buffer = buffer;
}

// / CLASSES ENFANTS: Connection__

Connection::Connection(): Message(){}
Connection::Connection(std::string &buffer): Message(buffer){}
Connection::Connection(Connection const& cpy): Message(cpy){}
Connection::~Connection(){}
Connection &Connection::operator=( Connection const &rhs )
{
    if (this != &rhs)
    {
        this->_buffer = rhs._buffer;
    }
    return (*this);
}