#include "server.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
		return (1);
	else
	{
		int sock_serv = atoi(av[1]);
		Server serv(sock_serv, av[2]);
		serv.startServer();
	}
	return (0);
}