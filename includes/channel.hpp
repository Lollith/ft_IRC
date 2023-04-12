#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "irc.h"

class Channel{
	public:
		Channel( std::string name );

		std::string getName( void ) const;
	
		void addClient();


	private:
		
		std::string		_name;
		
		
		Channel( void );

};

#endif