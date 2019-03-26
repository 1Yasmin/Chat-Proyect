#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

using namespace std;

int main(){

  //crear socket
  int sock = socket(AF_INET,SOCK_STREAM,0);
  if(sock == -1)
    printf("error al crear socket");
  int port = 8080;
  string ip_addr = "127.0.0.1";
  sockaddr_in hint;
  hint.sin_family = AF_INET;
  hint.sin_port = htons(port);
  inet_pton(AF_INET,ip_addr.c_str(),&hint.sin_addr);
  //conectar
  int connectRes = connect(sock,(sockaddr*)&hint, sizeof(hint));
  if(connectRes == -1)
    printf("error al conectar");
  //loop
 char buf[4096];
 string userInput;
 do{
    //escribir
    cout << ">";
    getline(cin, userInput);
    //convertir a jason

    //mandar al servidor
    int sendRes = send(sock, userInput.c_str(),userInput.size()+1,0);
    //revisar si fallo
    if(sendRes == -1)
      printf("no se pudo enviar el mensaje");
      continue;
    //respuesta del servidor
    memset(buf,0,4096);
    int bytesReceived = recv(sock,buf, 4096,0);
    //mostrar respuesta
    cout << "SERVER" << string(buf,bytesReceived) <<"\r\n";

  }while(true);
  //cerrar socket
  close(sock);
  return 0;
}
