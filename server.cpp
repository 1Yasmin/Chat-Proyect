#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define DEFAULT_PORT 8080;

int main(int argc, char *argv[]) {

  int port, socketfd;
  char buffer[255]; // buffer de información

  char *port_text;
  port_text = argv[1];

  // Si no se especifica el puerto se asigna el 8080
  if (port_text == NULL){
    port_text = (char*)DEFAULT_PORT;
  }

  // De char a int
  port = atoi(port_text);

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
  listen(socketfd, 20);
  clilen = sizeof(cli_addr);

  // Hacer un thread que siempre este escuchando este puerto para conexiones
  // Dentro de ese thread se hacer el accept y se crea un nuevo objeto usuario con su puerto y se agrega a una lista de usuarios


  return 0;
}