#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "irc.hpp"
class Client;
class Channel{
	public:
		Channel( std::string name );


		std::vector<Client *>	getClients( void ) const;
		std::string				getName( void ) const;

		std::string				getTopic( void ) const;
		void					setTopic( std::string newtopic );
		
		bool					get_flag_erase_chan( void ) const;
		void					set_flag_erase_chan( bool );

		bool 					has_clients(Client *client);

		void 					addClient(Client *client);
		void 					deleteClientFromChan(Client *client);

		void 					search_new_ope(Client *client);
		void 					check_vctor(Client *);

		
	private:
		std::vector<Client *>	_clients;
		std::string				_name;
		bool					_flag_erase_chan;
		std::string				_topic;
		
		
		Channel( void );

};

#endif