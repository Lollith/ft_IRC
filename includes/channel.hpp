#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "irc.hpp"
class Client;

class Channel{
	public:
		Channel( std::string name );
		~Channel( void );
		Channel( Channel const &copy );
		Channel &operator=( Channel const &rhs );


		std::vector<Client *>	getClients( void ) const;
		std::string				getName( void ) const;

		std::string				getTopic( void ) const;
		void					setTopic( std::string newtopic );
		
		bool					get_flag_erase_chan( void ) const;
		void					set_flag_erase_chan( bool );

		time_t					get_topic_time( void ) const;
		void					set_topic_time( time_t timestamp );
		
		time_t					get_create_time( void ) const;
		void					set_create_time( time_t timestamp );
		
		std::string				*get_mode( void );
		void					set_mode(std::string &mode);
		
		bool 					has_clients(Client *client);
		bool 					has_clients(std::string &newclient);
		Client 					*searchClient(std::string &name);

		bool					is_invited(Client *client);
		void 					set_invite(Client *client);

		void 					addClient(Client *client);
		void 					deleteClientFromChan(Client *client);

		void 					search_new_ope(Client *client);
		void 					check_vctor(Client *);

		
	private:
		std::vector<Client *>	_clients;
		std::string				_name;
		bool					_flag_erase_chan;
		std::string				_topic;
		time_t					_topic_time;
		std::string				_mode[3]; // 0 = i, 1 = s, 2 = t
		std::string				_create_time;
		std::string				_invited;
		
		Channel( void );

};

#endif