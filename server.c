#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg){
    //errorno
    perror(msg);
    exit(1);

}

int main(int argc, char *argv[]){

    if(argc < 2){
        fprintf(stderr,"No se especifico el puerto\n");
        exit(1);
    }

    int sockfd, newsockfd, portno, n;
    char buffer[255];
    //internet address
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (sockfd < 0)
        error("Error abriendo socket");
    //clear
    bzero((char *) &serv_addr, sizeof(serv_addr));
    //get portno
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    //host to network short htons
    serv_addr.sin_port = htons(portno);
    //bind
    if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        error("fallo en bind");
    }
    //segundo parametro indica la cantidad maxima de clientes
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    //aceptar conexion nuevo file descriptor
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    //check for failure
    if(newsockfd < 0){
        error("Error al aceptar conexion");
    }
    while(1){
        bzero(buffer,255);
        n = read(newsockfd, buffer, 255);
        if (n < 0){
            error("fallo en lectura");
        }
	    //show message
        printf("Client: %s\n",buffer);
        bzero(buffer,255);
        fgets(buffer,255, stdin);

        //reply del servidor
        n = write(newsockfd, buffer, strlen(buffer));
        if (n < 0) 
	        error("Error al escribir");

        int i = strncmp("bye", buffer, 3);
	    //terminar conexion
	    if (i == 0){
	        break;
	    }
    //end of while
    }

    close(newsockfd);
    close(sockfd);
    return 0;

}
