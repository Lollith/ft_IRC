# ft_IRC

# Plan de bataille 

1. 
	<!-- - initialisation - debut send-recev - sockets -select -->
	<!-- 1.0. recevoir Myport en arg => string en int -->
	<!-- 1.0 reparer port pour pouvoir accepter n -->

	<!-- 1.1. recevoir et parcer un message envoye -->

	<!-- 1.2 blocage - select() -->

	<!-- 1.3. client_socket qui increment =>  close qq part??? -->

	1.4 quitter proprement : disconnecting and reconnecting servers

	1.5 check leaks, fd opens,....

	<!-- 1.6 parsing port + passeword -->

2. authentification 
- CAP message
- AUTHENTICATE message
- PASS message
- NICK message
- USER message
- PING message
- PONG message
- OPER message
- QUIT message

3. Channel Operations
<!-- - channels = group d un ou +ieurs clients, receive all message; creee lorsque le 1er client joins et rm when last leave, name rules -->
<!-- - JOIN message -->
<!-- - PART message -->
<!-- - topic message : mode +t -->
<!-- - names message chan +s  user+i -->
<!-- - list message chan +s -->
<!-- - invite message chan +i -->


<!-- - JOIN = create ou se connecter  -->
<!-- - To create a new channel or become part of an existing channel, a user is required to join the channel using the JOIN command. If the channel doesn’t exist prior to joining, the channel is created and the creating user becomes a channel operator -->
<!-- RPL_TOPIC, NAMREPLY, ENDOFNAMES -->


<!-- 1 user => join serveral channel ( 1 limite) -->

4. messages prives
<!-- -	privmsg -->
<!-- - notice -->
	
	- ERR_CANNOTSENDTOCHAN (404)=>  This is generally sent in response to channel modes, such as a channel being moderated and the client not having permission to speak on the channel, or not being joined to a channel with the no external messages mode set.

5. server operators // non implementes 
allowed to perform maintenance: 
	- KILL = close the connection between a client and server,.... 
	can disconnect and reconnect servers:
	- SQUIT = disconnect a server
	- RESTART = reconnect

- Channel moderators are identified by the channel member prefix ('@' for standard channel operators
1er connecte sur le chan
	<!-- - KICK: Eject a client from the channel, -->
	- MODE: Change the channel’s modes,
	<!-- - INVITE: Invite a client to an invite-only channel (mode +i), -->
	<!-- - TOPIC: Change the channel topic in a mode +t channel,   -->
	

	
	



//-----------------------------PORTS--------------------------------------------
Tous les port en dessous de 1024 sont réservés.
Utiliser n'importe quel port au dessus de 1024 jusqu'à 65535.

/connect localhost 3333 123
/join #canal
/nick newname
/msg pseudotosend monmessage + ctrl n

~ nc localhost <port>
PASS <pass>
NICK <nick>
USER <nick> <user> <user> <user>


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



//-----------------------commande-------------------------------------------------
nc : Privmessage :
: <nickname> PRIVMSG <#chan> :<message>

info topic
/topic #chan

mode:
attention user en +i a la connexion

invite
nimporte qui peut invite
sauf si mode chan +i (invite only) => seulment operator 

#test

POur vérifier les ports écouté: 
//-----------------------commande-------------------------------------------------
ps -a 

sudo lsof -nP -iTCP -sTCP:LISTEN

sudo ss -tulpn