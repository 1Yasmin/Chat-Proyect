#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <cstring>
#include <iostream>
#include <time.h>

#include "packages/json.hpp"
#include "utils.cpp"
#include "user.cpp"
#include "json_maker.cpp"


using namespace std;
using json = nlohmann::json;

void error(const char *msg){
    perror(msg);
    exit(0);
}


int main(int argc, char *argv[]){
    json request_connection;
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[1024];
    
    if(argc < 3){
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(1);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
        error("error al abrir socket");

        server = gethostbyname(argv[1]);
        if(server == NULL)
            fprintf(stderr, "error, no existe el host");
        
	
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
        serv_addr.sin_port = htons(portno);
        if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
            error("Fallo en la conexion");
       
	//Envio del nombre del usuario
	char *username = argv[3];
        request_connection = send_connection(username);
        char *request_connect = to_char(request_connection);
        printf(request_connect);
        
	strcpy(buffer, request_connection.dump().c_str());
	n = write(sockfd,buffer,1024);
        n = read(sockfd, buffer, 1024);
          if (n < 0)
		error("Error al leer");
        printf("\nServer: %s",buffer);
	
	if (n < 0)
		error("Error en escritura");

	while(1){
		printf("\n escriba el mensaje:");
		bzero(buffer, 1024);
		fgets(buffer, 1024,stdin);
		n = write(sockfd, buffer, 1024);
		if (n < 0)
		    error("Error en escritura");
		bzero(buffer, 1024);
		n = read(sockfd, buffer, 1024);
	  	if (n < 0)
		    error("Error al leer");
		printf("Server: %s",buffer);
	//int i = strncmp("bye",buffer,3); 
	//if (i == 0)
	  //   break;
	}
	//close(sockfd);
	return 0;
}
