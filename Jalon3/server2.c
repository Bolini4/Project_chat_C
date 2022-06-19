#include "msg_struct.h"

#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BACKLOG 2

typedef struct pthread_arg_t {
    int new_socket_fd;
    struct sockaddr_in client_address;
    struct message message;
    struct message *pmessage;
   //l 116 139. */
} pthread_arg_t;

/*fonction Thread routine . */
void *pthread_routine(void *arg);

/* Signal handler to handle SIGTERM and SIGINT signals. */
void signal_handler(int signal_number);

int main(int argc, char *argv[]) {
    int port, socket_fd, new_socket_fd;
    struct sockaddr_in address;
    pthread_attr_t pthread_attr;
    pthread_arg_t *pthread_arg;
    pthread_t pthread;
    socklen_t client_address_len;

    /* Get port from command line arguments or stdin. */
    port = argc > 1 ? atoi(argv[1]) : 0;
    if (!port) {
        printf("Enter Port: ");
        scanf("%d", &port);
    }

    /* Initialise IPv4 address. */
    memset(&address, 0, sizeof address);
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

    /* Create TCP socket. */
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    /* Bind address to socket. */
    if (bind(socket_fd, (struct sockaddr *)&address, sizeof address) == -1) {
        perror("bind");
        exit(1);
    }

    /* Listen on socket. */
    if (listen(socket_fd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    /* Assign signal handlers to signals. */
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        perror("signal");
        exit(1);
    }
    if (signal(SIGTERM, signal_handler) == SIG_ERR) {
        perror("signal");
        exit(1);
    }
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        perror("signal");
        exit(1);
    }

    /* Initialise pthread attribute to create detached threads. */
    if (pthread_attr_init(&pthread_attr) != 0) {
        perror("pthread_attr_init");
        exit(1);
    }
    if (pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_DETACHED) != 0) {
        perror("pthread_attr_setdetachstate");
        exit(1);
    }

    while (1) {
        /* Create pthread argument pour chaque client. */
        pthread_arg = (pthread_arg_t *)malloc(sizeof *pthread_arg);
        if (!pthread_arg) {
            perror("malloc");
            continue;
        }

        /* Accept connection to client. */
        client_address_len = sizeof pthread_arg->client_address;
        new_socket_fd = accept(socket_fd, (struct sockaddr *)&pthread_arg->client_address, &client_address_len);
        if (new_socket_fd == -1) {
            perror("accept");
            free(pthread_arg);
            continue;
        }

        /* Initialise pthread argument. */
        pthread_arg->new_socket_fd = new_socket_fd;


        /* Create thread to serve connection to client. */
        if (pthread_create(&pthread, &pthread_attr, pthread_routine, (void *)pthread_arg) != 0) {
            perror("pthread_create");
            free(pthread_arg);
            continue;
        }
    }

    return 0;
}

void *pthread_routine(void *arg) {
    pthread_arg_t *pthread_arg = (pthread_arg_t *)arg;
    int new_socket_fd = pthread_arg->new_socket_fd;
	int n;
    struct sockaddr_in client_address = pthread_arg->client_address;
    struct message message;
	char buffer[255];
    char nick[] = { '/', 'n', 'i', 'c', 'k' };
    const char s[2] = " ";
    char *token;
    /*l22 116 */

    free(arg);
	    while (1)
    {
        bzero(buffer, 255);
        printf("Clear BUffer\n");
        n=read(new_socket_fd, buffer, strlen(buffer));
        printf("i have read\n");


/*on vérifie si on a entré une commande. */
        if(strcmp("/quit",message.infos)==0)
            printf("ça à marché");

        if (buffer[0]=='/'){
            printf("je suis rentré dans le /");

            if (buffer[1] =='q' ){
            printf("Connection terminated by user\n");
            break;

           }
            if (buffer[1] =='n' ){
            printf("Je suis dans le /nick \n");
            n = write(new_socket_fd, buffer, strlen(buffer));
           // token = strtok(buffer,s);
           // printf("%s",token);

            }
            printf("je suis dans aucun if ?");
        }
            printf("sorti des if /\n");

        if(buffer[0] != '/'){
            printf("Client : %s\n", buffer);
            n = write(new_socket_fd, buffer, strlen(buffer));
        }
        printf("je suis sorti du else ?\n");
    }


    close(new_socket_fd);
        printf("j'ai close socke ?");
    return NULL;
}

void signal_handler(int signal_number) {
    /* TODO: Put exit cleanup code here. */
    exit(0);
}