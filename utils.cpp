#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

char *read_socket(int socketfd, int buffer_size) {
  char *data;
  int a = read(socketfd, data, buffer_size);
  if (a < 0) {
    return NULL;
  }
  return data; 
}