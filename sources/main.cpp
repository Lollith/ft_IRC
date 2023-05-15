#include "irc.hpp"

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
	if (ac != 3)
	{
		std::cout << "need more parameters" <<std::endl;
		return (1);
	}
	else
	{
		int sock_serv = atoi(av[1]);
		if (sock_serv < 1024 || sock_serv > 65535)
		{
			std::cout << "bad port : 1024-65535" << std::endl;
			return 1;
		}
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
		if(serv.loop_recept_send() == false)
		{
			std::cout << "byebye"<< std::endl;
			return(1);
		}
	}
	return (0);
}