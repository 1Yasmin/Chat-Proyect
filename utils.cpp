#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void *read_socket(void *socketfd, void *buffer_size) {
  int socket = * (int *) socketfd; // cast el socket a int
  int buffer = * (int *) buffer_size; // cast el tamaño del buffer a int 
  char *data;
  if (read(socket, data, buffer) < 0) {
    printf("Error al leer socker %d", socket);
    return (void *) NULL;
  }
  return (void *) data; 
}