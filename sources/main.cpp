#include <stdio.h>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> //htons +define the in_addr cf <netinet/in.h>

#define MYPORT 3490 // port du client, creation - bind()
#define BACKLOG 10	/* Le nombre maxi de connections en attente  */

char *buf;

int main(int ac, char **av)
{
	int socket_fd; // listen sur socket_fd
	int new_fd;	   // conncetion sur
	struct sockaddr_in my_addr;
	struct sockaddr_in client_addr; // info adresse du client
	socklen_t sin_size;

	if (ac != 3)
		return (1);

	// creer un socket -descirpteur de fichier
	// https://man7.org/linux/man-pages/man2/socket.2.html
	// AF_INET: IPv4, SOCK_STREAM :TCP, IPPPROTO_TCP :TCP => 0 Indicates
	// that the default protocol for the type selected is to be used. For example,
	// IPPROTO_TCP is chosen for the protocol if the type was set to SOCK_STREAM
	// and the address family is AF_INET.
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1)
	{
		perror("socket"); // interdite ?
		return (1);
	}

	// 	// associer la socket avec un port de votre machine locale.
	my_addr.sin_family = AF_INET;		  // host byte order
	my_addr.sin_port = htons(MYPORT);	  // short, network byte order
	my_addr.sin_addr.s_addr = INADDR_ANY; // auto-remplissage avec  mon IP//inet_addr("127.0.0.1");
	bzero(&(my_addr.sin_zero), 8);		  // interdite?

	// , nous devons appeler bind() avant d'appeler listen() ou sinon le système va écouter sur un port au hasard.
	// int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	if (bind(socket_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind");
		return (1);
	}
	// les connections vont attendre dans la file de BACKLOG jusqu'à ce que vous
	// les acceptiez avec accept()
	if (listen(socket_fd, BACKLOG) == -1)
	{
		perror("listen");
		return (1);
	}
	while (1)
	{
		sin_size = sizeof(struct sockaddr_in);
		new_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &sin_size);
		if (new_fd == -1)
		{
			perror("accept"); // interdite ?
		}
		if (connect(new_fd, (struct sockaddr *)&my_addr, sin_size) == -1)
		{
			perror("connexion with client failed.");
		}
		send(new_fd, "HI", 3, 0);
		recv(new_fd, buf, 20, 0);
		printf("%s", buf);
	}

	return (0);
}