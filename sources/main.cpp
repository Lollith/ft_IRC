#include "irc.h"

int main(int ac, char **av)
{
	char *buf;

	if (ac != 3)
		return (1);
	else
	{
		int sock_serv = atoi(av[1]);
		Server serv(sock_serv, av[2]);
		if (!serv.startServer())
		{
			std::cout << "startServer failed" << std::endl;
			return 1;
		}
		serv.loop_recept_send();
	}
	return (0);
}