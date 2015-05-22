/*
*UDP Socket Send and Receive Functionsheader File
*@author Love Bisaria
*date: September 8th , 2014 6:07 PM
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/wait.h>
#include <netdb.h>


void sendStuff(int newsockfd , struct sockaddr_in to , int tolen);
void receiveStuff(int newsockfd , struct sockaddr_in from , int fromlen);

