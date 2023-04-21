#include "irc.h"

//__________________________________________________________
// en globale: définition dun ptr sur objet Server dans le but
// qu il soit accessible depuis sigHandler
//__________________________________________________________
Server *sp = NULL;

void sigHandler(int sig)
{
	(void) sig;
	sp->stop();
}

int main(int ac, char **av)
{
	// char *buf;

	if (ac != 3)
		return (1);
	else
	{
		int sock_serv = atoi(av[1]);
		Server serv(sock_serv, av[2]);

		struct sigaction act;
		act.sa_handler = sigHandler;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		sigaction(SIGINT, &act, 0);

		if (!serv.startServer())
		{
			std::cout << "startServer failed" << std::endl;
			return 1;
		}
		sp = &serv; // sp pointe sur l'instance serv
		serv.loop_recept_send();
	}
	return (0);
}