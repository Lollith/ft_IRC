# ft_IRC

Plan de bataille - a revoir

<!-- 1.0. recevoir Myport en arg => string en int -->
1.0 reparer port pour pouvoir accepter n

1.1. recevoir et parcer un message envoye

1.2 blocage - select()

1.3 quitter proprement

2. authentification 

3. chan

4. autre : messages prives,....



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
