# ft_IRC

# Plan de bataille 

1. initialisation - debut send-recev - sockets -select
	<!-- 1.0. recevoir Myport en arg => string en int -->
	1.0 reparer port pour pouvoir accepter n

	1.1. recevoir et parcer un message envoye

	<!-- 1.2 blocage - select() -->

	1.3. client_socket qui increment =>  close qq part???

	1.4 quitter proprement : disconnecting and reconnecting servers

	1.5 check leaks, fd opens,....


2. authentification 
	nickname (cf rules name, unique)+ username

3. channels = group d un ou +ieurs clients, receive all message; creee lorsque le 1er client joins et rm when last leave, name rules
	JOIN = create ou se connecter => 1er connecter devient Operators
	KICK: Eject a client from the channel
	MODE: Change the channel’s modes
	INVITE: Invite a client to an invite-only channel (mode +i)
	TOPIC: Change the channel topic in a mode +t channel

	1 user => join serveral channel ( 1 limite)

4. messages prives

5. operators // utilisateurs basiques: 
	allowed to perform maintenance: 
	KILL = close the connection between a client and server,.... 

	can disconnect and reconnect servers
	SQUIT = disconnect a server
	RESTART = reconnect
	



//-----------------------------PORTS--------------------------------------------
Tous les port en dessous de 1024 sont réservés.
Utiliser n'importe quel port au dessus de 1024 jusqu'à 65535.


//------------------------------------------------------------------------------
definition de la structure ( contenue ds la lib )
```c
#include <arpa/inet.h>

struct sockaddr {
         unsigned short    sa_family;    /* famille d'adresse, AF_xxx        */
         char              sa_data[14];  /* 14 octets d'adresse de protocole */
 };

 struct sockaddr_in
 {
 	short int			sin_family; //famille d adresse
 	unsigned short int	sin_port; // num du port  a contacter
 	struct in_addr		sin_addr; //adresse internet de lhote
 	unsigned char		sin_zero[8];// contient uniquement des zéros (étant donné que l'adresse IP et le port occupent 6 octets, les 8 octets restants doivent être à zéro)
 };```
