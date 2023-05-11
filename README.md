# ft_IRC

## Plan de bataille 

1. communication
	- initialisation - debut send-recev - sockets -select
	- recevoir et parcer un message envoye


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
	- channels 
	- JOIN message
	- PART message
	- topic message : mode +t
	- names message chan +s  user+i
	- list message chan +s
	- invite message chan +i


4. messages prives
	- privmsgs
	- notices


5. Channel moderators are identified by the channel member prefix ('@' for standard channel operators
1er connecte sur le chan
	- KICK: Eject a client from the channel,
	- MODE: Change the channel’s modes,
	- INVITE: Invite a client to an invite-only channel (mode +i),
	- TOPIC: Change the channel topic in a mode +t channel,  



## PORTS

Tous les port en dessous de 1024 sont réservés.
Utiliser n'importe quel port au dessus de 1024 jusqu'à 65535.

- ```/connect localhost 3333 123```
- ```/join #canal```
- ```/nick newname```
- ```/msg pseudotosend monmessage + ctrl n```

- ```~ nc localhost <port>```
- ```PASS <pass>```
- ```NICK <nick>```
- ```USER <nick> <user> <user> <user>```


 ## COMMANDES

### mode
- mode user initialised : ```+i``` 
- user mode : ```i```
- chan mode : ```i```/```s```/```t```

### privmsg :
- nc: ```<nickname> PRIVMSG <#chan> :<message>```
- ```/msg <target>,<target> <texte>```
- ```/notice <target>,<target> <texte>``` 
- target = #chan/ nick

### join :
- ```/join <chan>[,<chan>]```

### part :
- ```/part <chan>[,<chan>] [reason]```

### topic
- change the topic : ```/topic #chan newtopic```
- info: ```/topic #chan```
- chan mode ```+t``` : ```protected topic: only operator chan can change```

### names
- view nicknames joined on chan
- ```/names <channel>[,channel] ```
- chan mode ```+s``` : secret : if user not on chan => can 't view nicknames
- user mode ```+i``` : invisible : if user not on chan => invisble user is not shown

### list
- list of all visible channels  + info : ```/list```
- info of the given chans : ```/list <chan>,<chan>```
- chan mode ```+s``` : secret : chan not shown

### invite
- invite user on chan
- ```/invite <nickname><channel>```
- chan mode ```+i``` : invite only : only channel operator can invite

### kick
- ```/kick <chan> <user>,<user> [comment]```


## STRUCTURE

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
 };