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


int status_options(){
	int select;
	cout<<endl;
	cout<< "Eliga el numero del nuevo status"<<endl;
	cout<<" 0. Activo"<< endl;
	cout<<" 1. Ocupado"<<endl;
	cout<<" 2. Inactivo"<<endl;
	cin>>select;
	return select;
}


int menu(int argc, char *argv[]){
	int select;
	cout<<endl;
	cout<<" Seleccione una opcion"<<endl;
	cout<<"1. Broadcasting"<<endl;
	cout<<"2. Chatear con un usuario"<<endl;
	cout<<"3. Cambiar de status"<<endl;
	cout<<"4. Lista de usuarios conectados"<<endl;
	cout<<"5. informacion de un usuario"<<endl;
	cout<<"6. Ayuda"<<endl;
	cout<<"7. Salir"<<endl;
	cin >> select;
	return select;
}

string broadcast(){
	string msg;
	cout << "Escriba el mensaje: ";
	getline(cin,msg);
	return msg;	
}

void help_menu(){
	cout<<"   Menu de ayuda"<<endl;
	cout<<"1. Envia un mensaje a todos"<<endl;
	cout<<"2. Envia un mensaje a 1 usuario en especifico con su id"<<endl;
	cout<<"3. Cambia a activo, ocupado o inactivo"<<endl;
	cout<<"4. Ve quienes estan conectados"<<endl;
	cout<<"5. Ve los datos de un usuario"<<endl;
	cout<<"6. Ayuda"<<endl;
	cout<<"7. Desconectarse del servidor"<<'\n';
}
