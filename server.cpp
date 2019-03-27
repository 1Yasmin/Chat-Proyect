#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#include "packages/json.hpp"
#include "utils.cpp"
#include "user.cpp"

using json = nlohmann::json;

std::vector<User *> users;
const int DEFAULT_PORT = 8080;
const int BUFFER_SIZE = 1024;



void *accept_connection(void *socketfd, void *cli_addr_v, void *clilen_v) {
  char *result = (char *) read_socket(socketfd, (void *) BUFFER_SIZE);
  struct sockaddr *cli_addr = (struct sockaddr *) cli_addr_v;
  socklen_t *clilen = (socklen_t *) clilen_v;
 
  if (result == NULL) {                 // Si hay un error 
    printf("No se pudo conectar!");
    return (void *) -1;
  }

  json response = json::parse(result);
  int code = response["code"];
  string username = response["data"]["username"];

  switch (code)
  {
    case 0: // request connection
      int user_socket;
      user_socket = accept(*(int *) socketfd, cli_addr, clilen);    // generar el socket para el usuario A
      if (user_socket < 0)
        return (void *) -1;
      // crear nuevo usuario agregarlo al array
      User new_user;
      new_user.init(user_socket, (char *) &username[0]); // crear al nuevo usuario
      users.push_back(&new_user); // se añade el nuevo usuario
      return (void *) 0;
    default:
      return (void *) -1;
  }

  printf("%s\n",response["code"]);

  // convertir a json y verificar que la acción sea conexión si es conexión hacer accept
  //    y devolver el nuevo socket
  // de lo contrario retornar -1 para indicar un error


  return (void *) result;
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

  //aceptar conexion nuevo file descriptor
  int newsockfd = accept(socketfd, (struct sockaddr*) &cli_addr, &clilen);
  //check for failure
  if (newsockfd < 0){
    printf("Error al aceptar conexion");
  }

  // Hacer un thread que siempre este escuchando este puerto para conexiones
  // Dentro de ese thread se hacer el accept y se crea un nuevo objeto usuario con su puerto y se agrega a una lista de usuarios
  

  return 0;
}