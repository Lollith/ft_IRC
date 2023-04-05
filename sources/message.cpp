#include "message.hpp"

//__________________________________________________canonic form
Message::Message( void ):_buffer("WELCOME!\n"){}

Message::Message( std::string &buffer ): _buffer(buffer){
}


Message::Message( Message const &copy ){
	*this = copy;
}

Message::~Message( void ) { }

Message &Message::operator=( Message const &rhs )
{
	if (this != &rhs)
	{
		this->_buffer = rhs._buffer;
	}
	return (*this);
}

//__________________________________________________GETTERS_SETTERS
std::string Message::getBuffer( void )const
{
	return(_buffer);
}