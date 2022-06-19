
//
// Created by matte on 19/05/2022.
//
/*
 filename server_ipadress portno
 argv[0] filename
 argv[1] server_ipadress
 argv[2] portno
 ---------------------------------------------------------------------
 #include <netinet/in.h>
struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET
    unsigned short   sin_port;     // e.g. htons(portno) ATTENTION PORTNO Doit avoit atoi
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};
struct in_addr {
    unsigned long s_addr;  // load with inet_aton()
};
-----------------------------------------------------------------------
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h> //inclusion threads

#define MAX_CLIENT 5

void *create_thread(){

}

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    if(argc< 2)
    {
        fprintf(stderr, "Port no not provided. Program terminated\n");
        exit(1);
    }
    int sockfd, newsockfd, portno,n; //tout ce qui est lié à la connexion
    char buffer[255]; //buffer qui va stocker les messages

    struct sockaddr_in serv_addr, cli_addr; //top docuement structure de sockaddr
    socklen_t clilen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0); // socket(int domain, int type, int protocol); -> renvoie un int
    //AF_INET = Protocole IPV4, SOCK_STREAM = support de comm garantisasnt intégrité. Protocol reste à 0
    // Si socket renvoie <0 c'est que y'a erreur dans la création du socket, sinon renvoie un descrupteur.
    if(sockfd < 0)
    {
        error("Error Opening Socket.");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr)); //met à 0 srv_addr
    portno = atoi(argv[1]); //il faut appliquer atoi au num de port pour que la machine comprenne.

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);  //reagrder plus haut, on appluqe les varaibles à la structure

    if(bind(sockfd , (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
        error("Binding Failed.");
    listen(sockfd, MAX_CLIENT); //on dit que sockfd est un socket d'écoute avec 5 connexions simultanées
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    // int accept(int sockfd, struct sockaddr *adresse, socklen_t *longueur);
    // renvoie un token identifiant le nouveau socket

    if(newsockfd < 0)
        error ("Error on Accept");

    while (1)
    {
        bzero(buffer, 255);
        n=read(newsockfd, buffer, 255);
        if (n < 0)
            error("Error on reading.");

        printf("Client : %s\n", buffer);
        //bzero(buffer, 255);
        // fgets(buffer, 255, stdin);

        n = write(newsockfd, buffer, strlen(buffer));
        if (n < 0)
            error ("Error on writing");

        if (strcmp(buffer, "/quit\n") ==0 ){
            printf("Connection terminated by user\n");
            break;
        }
    }
    close(newsockfd);
    close(sockfd);
    return 0;
}