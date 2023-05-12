#!bin/bash

# Pour fermer tous les terminaux ouverts :
# pkill -f "terminator"

SERVER="localhost"
PORT="6667"
PASS="a"
NICK="user"
ID=1

while [ $ID -le 100 ]
do
	echo "pass $PASS
	nick user$ID
	user user$ID 0 * :user$ID" | nc $SERVER $PORT &
	# terminator -e "irssi -c $SERVER -p $PORT -w $PASS -n $NICK$ID " &
	ID=`expr $ID + 1`
done

