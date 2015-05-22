/*
*UDP Socket Send and Receive Functions Description
*@author Love Bisaria
*date: September 8th , 2014 6:07 PM
*/

#include "sendAndReceiveFunctions.h"

void sendStuff(int sockfd , struct sockaddr_in to , int tolen)
{
    printf("\nInside the code of SendStuff\n");
    char buffer[256];
    bzero(buffer,256);

    //strcpy(buffer,"I got your message");
    printf("\nPlease Enter a Message.\n");
    scanf("%s",buffer);
    printf("\nBuffer for SendStuff: %s",buffer);
    int n = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&to, tolen);
    printf("\nValue of n after sendto: %d\n", n);
    if (n < 0)
        error("ERROR on sendto");
    //close(sockfd);
}

void receiveStuff(int newsockfd , struct sockaddr_in from , int fromlen)
{
    printf("\nInside the code of ReceiveStuff\n");
    char buffer[256];
    bzero(buffer,256);

    //Receiving the Message
    int n=recvfrom(newsockfd, buffer, 1024, 0, (struct sockaddr *)&from, &fromlen);
    if (n < 0)
        error("ERROR on recvfrom");
    printf("\nBuffer for ReceiveStuff: %s",buffer);
    //Received Message
    fprintf(stdout, "Here is the message: %s\n",buffer);
    fflush(stdout);
    //close(newsockfd);
}
