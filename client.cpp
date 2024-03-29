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

void *read_socket(void *socket) {
    int sockfd = *(int *) socket;
    while(1) {
        char *response = (char *) malloc(sizeof(char) * 1024);
        read(sockfd, response, 1024);
//        string response_str = response;
	int code = get_code(response);
	// Broadcasting	
	if (code == 201){
		cout<<"Chat Broadcasting"<<endl;
		json response_msg = json::parse(response);
	        string user_from = response_msg["data"]["from"];
	        string user_msg = response_msg["data"]["message"];
		cout<<user_from<<" Mensaje:  "<<user_msg<<endl;
	}
	//cambio de estado
	else if(code== 204){
		cout<<"El estado se cambio exitosamente"<<endl;	
	}
	// lista de usuarios
	else if (code == 203){
		json response_users = json::parse(response);
	        json users_list = response_users["data"]["users"];
		cout<<"Los usuarios conectados son: "<<endl;
                for (json::iterator it = users_list.begin(); it != users_list.end(); ++it ){
			json temp_user = *it;
			cout<<"username: "<<temp_user["username"]<<"  id: "<<temp_user["id"]<< "  status:"<<temp_user["status"] <<endl;
			}


	}
        /*if (!response_str.empty()) {
            printf("Server %s\n", response_str.c_str());
        }*/
        free(response);
    }
    return (void *)0;
}


int main(int argc, char *argv[]) {
    json request_connection;
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[1024];

	
    
    if (argc < 3){
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(1);
    }
    portno = atoi(argv[3]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) 
        error("error al abrir socket");

	server = gethostbyname(argv[2]);
	if (server == NULL)
		fprintf(stderr, "error, no existe el host");
	

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
		error("Fallo en la conexion");
       
	//Envio del nombre del usuario
	char *username = argv[1];
        request_connection = send_connection(username);       
	strcpy(buffer, request_connection.dump().c_str());

	n = write(sockfd,buffer,1024);
        n = read(sockfd, buffer, 1024);
          if (n < 0)
		error("Error al leer");
	json response = json::parse(buffer);
 	int user_id = response["data"]["user"]["id"];
        printf("\nServer: %s",buffer);

	// read thread
	pthread_t read_thread;
	int thread_status = pthread_create(&read_thread, NULL, read_socket, (void *) &sockfd);  
	
	
	if (n < 0)
		error("Error en escritura");

	while (1) {
		int select = menu(argc, argv);
		// Broadcasting
		if(select == 1){
			bzero(buffer, 1024);
			fgets(buffer, 1024,stdin);
			string broadcast_msg = broadcast();
			json request_broadcast = request_send(username,broadcast_msg);
			//cout << request_broadcast<<endl;
			strcpy(buffer,request_broadcast.dump().c_str());
			n = write(sockfd,buffer,1024);
			if (n < 0)
			    error("Error en escritura");
			bzero(buffer, 1024);
			printf("Server: %s",buffer);

		}
		// Chatear con un usuario 
		else if (select == 2) {
			int id; 
			char *msg = (char *) malloc(sizeof(char) * 1024);
			cout << "Ingrese el id del usuario con el que desea chatear" <<endl;
			cin >> id;
			cout << "Ingrese el mensaje privado:" <<endl;
			cin >> msg;
			json u = msg_privado(id, msg);
			strcpy(buffer, u.dump().c_str());
			n = write(sockfd, buffer, 1024);
			if (n < 0)
			    error("Error en escritura");
			bzero(buffer, 1024);
			printf("Server: %s",buffer);
			
		}
		// Cambiar status
		else if(select == 3){
			bzero(buffer, 1024);
			fgets(buffer, 1024,stdin);
			int new_status = status_options();
			json request_status = change_status(user_id, new_status);     
			strcpy(buffer, request_status.dump().c_str());
			n = write(sockfd, buffer, 1024);
			if (n < 0)
			    error("Error en escritura");
			bzero(buffer, 1024);


		}
		// Lista de usuarios conectados
		else if(select == 4){
			bzero(buffer, 1024);
			fgets(buffer, 1024,stdin);
			json request_usuarios = obtener_usuarios();     
			strcpy(buffer, request_usuarios.dump().c_str());
			n = write(sockfd, buffer, 1024);
			if (n < 0)
			    error("Error en escritura");
			bzero(buffer, 1024);/* 
			n = read(sockfd, buffer, 1024);
		  	if (n < 0)
			    error("Error al leer");
			json response = json::parse(buffer);
		        json users_list = response["data"]["users"];
			cout<<"Los usuarios conectados son: "<<endl;
                        for (json::iterator it = users_list.begin(); it != users_list.end(); ++it ){
                                json temp_user = *it;
				cout<<"username: "<<temp_user["username"]<<"  id: "<<temp_user["id"]<< "	status:"<<temp_user["status"] <<endl;
			} */
		}
		// Informacion de un usuario
		else if(select == 5){
			bzero(buffer, 1024);
			fgets(buffer, 1024,stdin);
			int user_id;
			cout<<"Ingrese el id del usuario: ";
			cin>>user_id;	
			
			json request_usuarios = obtener_usuario(user_id);     
			strcpy(buffer, request_usuarios.dump().c_str());
			printf("aosco als %s", request_usuarios.dump().c_str());
			sleep(1);
			n = write(sockfd, buffer, 1024);
			if (n < 0)
			    error("Error en escritura");
			bzero(buffer, 1024);
		}
		// Ayuda
		else if(select == 6){
			help_menu();
		}
		// Salir
		else if(select == 7){			
			cout<<"Adios"<<endl;
			bzero(buffer, 1024);
			fgets(buffer, 1024,stdin);
			json request_bye = request_goodbye();
			strcpy(buffer, request_bye.dump().c_str());

			n = write(sockfd, buffer, 1024);
			//if (n < 0)
			  //  error("Error en escritura");
			//bzero(buffer, 1024);
			break;
			
		}
	}
	//close(sockfd);
	return 0;
}
