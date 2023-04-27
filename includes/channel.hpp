#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "irc.h"

class Channel{
	public:
		Channel( std::string name );


		std::vector<Client *>	getClients( void ) const;
		std::string				getName( void ) const;

		bool					get_flag_erase_chan( void ) const;
		void					set_flag_erase_chan( bool );

		bool 					hasClient(Client *client);
		void 					addClient(Client *client);
		void 					deleteClientFromChan(Client *client);

	private:
		std::vector<Client *>	_clients;
		std::string				_name;
		bool					_flag_erase_chan;
		
		
		Channel( void );

};

#endif