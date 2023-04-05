#include "client.hpp"
#include "server.hpp"

//__________________________________________________canonic form

Client::Client(int sock_client): _socket_client(sock_client)
{
}

// Client::Client(Client const &cpy)
// {
// }

// Client &Client::operator=(Client const &rhs)
// {
// }

Client::~Client(void) {} // close() ou/et freeinfo() à faire?

//__________________________________________________GETTERS_SETTERS


int Client::getSocketClient()
{
	return this->_socket_client;
}