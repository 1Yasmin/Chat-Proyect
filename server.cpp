#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include "packages/json.hpp"
#include "utils.cpp"
#include "user.cpp"

using json = nlohmann::json;

std::vector<User *> users;
const int DEFAULT_PORT = 8080;
const int BUFFER_SIZE = 1024;

/*
int accept_connection(int socketfd) {
  char *result = read_socket(socketfd, BUFFER_SIZE);
  struct sockaddr_in cli_addr;
  socklen_t clilen;
  clilen = sizeof(cli_addr);

 
  if (result == NULL) {                 // Si hay un error 
    return -1;
  }


  json response = json::parse(result);
  int code = response["code"];
  string username = response["data"]["username"];
  
  printf("HOLA\n");

  switch (code)
  {
    case 0: // request connection
      if (socketfd < 0)
        return -1;
      // crear nuevo usuario agregarlo al array
      User new_user;
      new_user.init(socketfd, (char *) &username[0]); // crear al nuevo usuario
      users.push_back(&new_user); // se añade el nuevo usuario
      return 0;
    default:
      return -1;
  } 
  return 1;
}*/

void *check_messages(void * user_sock) {
  int sock = *(int *) user_sock;
  char *result = (char *) malloc(sizeof(char) * BUFFER_SIZE);
  while (1) {
    bzero(result, 1024);
    read(sock, result, BUFFER_SIZE);
    //write(sock, result, strlen(result));
    printf("%s\n", result);
    if (result == NULL) 
      printf("NO\n");

  }
  printf("Salio por alguna razon");
  return 0;
}

void *check_connections(void *socketfd) {
  int result;
  struct sockaddr_in cli_addr;
  socklen_t clilen;
  clilen = sizeof(cli_addr);
  int socket = *(int *) socketfd;
  pthread_t user_thread;
  int user_sock;
  while (1) {
    user_sock = accept(socket, (struct sockaddr *)&cli_addr, &clilen);    // generar el socket para el usuario A
    if (user_sock < 0) 
      printf("Error de conexión");
    printf("%d> Se conecto\n", user_sock);
    // crear thread que escucha el socket de este nuevo usuario
    if (pthread_create(&user_thread, NULL, check_messages, (void *) &user_sock) < 0)
      printf("error");
    //pthread_join(user_thread, NULL);
  } 



  return (void *) 0;
}

int main(int argc, char *argv[]) {

  int port, socketfd;
  char buffer[255]; // buffer de información

  char *port_text;
  port_text = argv[1];

  // Si no se especifica el puerto se asigna el 8080
  if (port_text == NULL){
    port = DEFAULT_PORT;
  } else {
    // De char a int
    port = atoi(port_text);
  }

  printf("Puerto %d\n", port);

  // Levantar el servidor
  socket(AF_INET, SOCK_RAW, 0);

  // internet address
  struct sockaddr_in serv_addr, cli_addr;
  socklen_t clilen;

  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd < 0)
    printf("Error abriendo socket");

  // clear buffer
  bzero((char *) &serv_addr, sizeof(serv_addr));

  // set socket struct
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  // host to network short htons
  serv_addr.sin_port = htons(port);

  // bind
  if(bind(socketfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
      printf("fallo en bind");

  // segundo parametro indica la cantidad maxima de clientes
  listen(socketfd, 5);
  clilen = sizeof(cli_addr);

  pthread_t connection_thread;


  int thread_status = pthread_create(&connection_thread, NULL, check_connections, (void *) &socketfd);  
  pthread_join(connection_thread, NULL);

  return 0;
}