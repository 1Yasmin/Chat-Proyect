#include <iostream>
#include <stdio.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */ 

#define DEFAULT_PORT 8080;

int main(int argc, char *argv[]) {

  int port, socket_fd;

  char *port_text;
  port_text = argv[1];

  // Si no se especifica el puerto se asigna el 8080
  if (port_text == NULL){
    port_text = (char*)DEFAULT_PORT;
  }

  // De char a int
  port = atoi(port_text);

  // Levantar el servidor
  socket(AF_INET, SOCK_RAW, 1);

  printf("%d\n", port);

  return 0;
}