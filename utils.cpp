#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

char *read_socket(int socketfd, int buffer_size) {
  char *data;
  int a = read(socketfd, data, buffer_size);
  if (a < 0) {
    return NULL;
  }
  return data; 
}


int menu(int argc, char *argv[]){
	int select;
	do{
		cout<<" Seleccione una opcion"<<endl;
		cout<<"1. Broadcasting"<<endl;
		cout<<"2. Chatear con un usuario"<<endl;
		cout<<"3. Cambiar de status"<<endl;
		cout<<"4. Lista de usuarios conectados"<<endl;
		cout<<"5. informacion de un usuario"<<endl;
		cout<<"6. Ayuda"<<endl;
		cout<<"7. Salir"<<endl;
		cin >> select;
		switch(select){
		  case 1: 
		    cout<< "opcion 1 seleccionada"<<endl;
		    break;
		  case 2: 
		    cout<< "opcion 2 seleccionada"<<endl;
		    break;
		  case 3: 
		    cout<< "opcion 3 seleccionada"<<endl;
		    break;
		  case 4: 
		    cout<< "opcion 4 seleccionada"<<endl;
		    break;
		  case 5: 
		    cout<< "opcion 5 seleccionada"<<endl;
		    break;
		  case 6: 
		    cout<< "opcion 6 seleccionada"<<endl;
		    break;
		  case 7: 
		    cout<< "opcion 7 seleccionada"<<endl;
		    break;
		  default:
		    break;
		}
	}
	while (select != 7);
	return 0;
}

