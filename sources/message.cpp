#include "message.hpp"

//__________________________________________________canonic form
Message::Message( void ):_buffer("WELCOME!\n"){
	// _buffer = "001 lollith :welcome to the localhost Network, lollith! lollith@localhost\r\n";
	//   "<client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]"
}

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
std::string const& Message::getBuffer( void )const
{
	return(_buffer);
}