/*
*UDP PEER
*@author Love Bisaria
*date: September 8th,2014 3:30 AM
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

#include "sendAndReceiveFunctions.h"

#define SENDPORT 5000
#define RECVPORT 5001

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void SigCatcher(int n){
    wait3(NULL, WNOHANG, NULL);
}

int main(int argc, char *argv[])
{

     //Variable Declaration
     int sockfd = -1;
     //int newsockfd = -1;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     //int n;
     struct hostent *server;


     //Fork Variables
     int p1;

     //Check for Port Number
     /*if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }*/

     //Get Socket File Descriptor

     clilen = sizeof(cli_addr);

     //Signal Handler to Kill the Zombie Process
     signal(SIGCHLD, SigCatcher);





        //Clearing the Buffer
    bzero(buffer,256);

        //Forking Code
    p1 = fork();
    if(p1==0)
    {
        printf("\n Inside P1!!\n");
        int sending = fork();

        if(0 == sending)
        {
            printf("\n\nInside the code for sending!!\n");

            struct sockaddr_in to;
            int tolen;
            /*Client Code for sending the data*/

            sockfd = socket(AF_INET, SOCK_DGRAM, 0);
            if (sockfd < 0)
                error("ERROR opening socket");
            printf("\nPeerB sending sockfd: %d\n",sockfd);

            server = gethostbyname("127.0.0.1");

            if (server == NULL)
            {
                fprintf(stderr,"ERROR, no such host\n");
                exit(0);
            }

            bzero((char *) &serv_addr, sizeof(serv_addr));

            serv_addr.sin_family = AF_INET;
            bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
            serv_addr.sin_port = htons(SENDPORT);


            /*fprintf(stdout, "Please enter the message: ");
            fflush(stdout);
            bzero(buffer,256);
            fgets(buffer,255,stdin);*/

            to= serv_addr;
            printf("\nPeerB sending ServerPort: %d\n", to.sin_port);
            tolen = sizeof(struct sockaddr);
            printf("\nPeerB sending tolen: %d\n",tolen);

            sendStuff(sockfd , to , tolen);

            //Exiting the Child
            close(sockfd);
            exit(0);
        }
        else
        {
            int receiving = fork();
            if(0 ==receiving )
            {
                printf("\n\n Inside the code for receiving!!\n");
                //Variable Declaration
                struct sockaddr_in from;
                int fromlen;

                /*Initialization of Socket Data Structures*/
                sockfd = socket(AF_INET, SOCK_DGRAM, 0);
                if (sockfd < 0)
                    error("ERROR opening socket");
                printf("\nPeerB receiving sockfd: %d\n",sockfd);

                //Clearing the Server Address Data Structure
                bzero((char *) &serv_addr, sizeof(serv_addr));

                //Setting the Port Number
                //portno = atoi(argv[1]);

                //Setting the Server Address Data Structure
                serv_addr.sin_family = AF_INET;
                serv_addr.sin_addr.s_addr = INADDR_ANY;
                serv_addr.sin_port = htons(RECVPORT);

                //Binding the Socket
                if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
                    error("ERROR on binding");
                printf("PeerB receiving Socket Binding Successful\n");
                //Listening to the Socket
                listen(sockfd,5);
                fromlen = sizeof(struct sockaddr_in);

                //Variable to store Sender's(Client's) Adress

                receiveStuff(sockfd , from , fromlen);
                close(sockfd);
                exit(0);
                }
            }
        }


        //Receiving the message into Buffer
       /* int n=recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr *)&from, &fromlen);
        if (n < 0)
            error("ERROR on recvfrom");

        //Received Message
        fprintf(stdout, "Here is the message: %s\n",buffer);
        fflush(stdout);

        //Sending the Messgae
        n = sendto(sockfd, "I got your message", 18, 0, (struct sockaddr *)&from, fromlen);
        if (n < 0) error("ERROR on sendto");*/


    //Closing the Socket
    close(sockfd);

    //Ending the Program
    return 0;
}
