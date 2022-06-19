#include "msg_struct.h"

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_NAME_LEN_MAX 255

int main(int argc, char *argv[]) {
	char buffer[255];
    char server_name[SERVER_NAME_LEN_MAX + 1] = { 0 };
    int server_port, socket_fd, n;
    struct hostent *server_host;
    struct sockaddr_in server_address;
    struct message *messages;

    messages = malloc(sizeof(messages));
    /* infos serveur. */
    if (argc > 1) {
        strncpy(server_name, argv[1], SERVER_NAME_LEN_MAX);
    } else {
        printf("Enter Server Name: ");
        scanf("%s", server_name);
    }

    /* Get server port from command line arguments or stdin. */
    server_port = argc > 2 ? atoi(argv[2]) : 0;
    if (!server_port) {
        printf("Enter Port: ");
        scanf("%d", &server_port);
    }

    /* avoir serveur host. */
    server_host = gethostbyname(server_name);

    /* initisalition infos ipv4 sreveur. */
    memset(&server_address, 0, sizeof server_address);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    memcpy(&server_address.sin_addr.s_addr, server_host->h_addr, server_host->h_length);

    /* crée le socket. */
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    /* connection au socket serveur. */
    if (connect(socket_fd, (struct sockaddr *)&server_address, sizeof server_address) == -1) {
		perror("connect");
        exit(1);
	}

	
	    while(1)
    {
        bzero(buffer, 255);
        fgets(buffer, 255, stdin); //écrit dans le buffer via stdin (entrée au clavier)
        n = write (socket_fd,buffer, strlen(buffer));
        if (strcmp(buffer, "/quit\n") ==0 ){
            printf("Connection terminated.\n");
            break;
        }
        if(n < 0)
            perror("Error on writing");
        bzero(buffer, 255);
        n = read(socket_fd , buffer , 255);
        if (n< 0)
            perror ("Error on reading");
        printf("Server : %s" , buffer);
    }



    close(socket_fd);
    return 0;
}