#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "irc.h"

class Channel{
	public:
		Channel( std::string name );


		std::vector<Client *>	getClients( void ) const;
		std::string				getName( void ) const;
	
		bool 					hasClient(Client *client);
		void 					addClient(Client *client);
		void 					deleteClientFromChan(Client *client);

	private:
		std::vector<Client *>	_clients;
		std::string				_name;
		
		
		Channel( void );

};

#endif