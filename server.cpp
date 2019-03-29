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
#include "json_maker.cpp"

using json = nlohmann::json;
using namespace std;

std::vector<User> users;
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

    if (result[0] == '{') { // es un json ?
      int code = get_code(result);
        json request = json::parse(result);
        // HANDSHAKE
        if (code == 0) {
          // check if username exists
          string username_s = request["data"]["username"];
          char *username = str_to_char(username_s);

          int is_in = username_duplicate(username, users);
      // string user = request["data"]["user"];
     //char* user_id = to_char(user)
          if (is_in < 0) {
            json failed = reject_connection();
            char *failed_c = to_char(failed);

            write(sock, failed_c, BUFFER_SIZE);

            delete[] failed_c;
            close(sock); // close connection
          } else {
            json success = accept_connection(sock, username, 0);
            char *success_c = to_char(success);

            write(sock, success_c, BUFFER_SIZE);

            User new_user;
            new_user.init(sock, username); // crear al nuevo usuario
            users.push_back(new_user); // se añade el nuevo usuario
          }
        }
        // SEND MESSAGE
        else if (code == 1) {
          // check if username exists
          json data = request["data"];
          char *username = (*get_user_by_id(sock, users)).username;
          string msg_str = data["message"];
          char *msg = to_char(msg_str);
          char *to_c = to_char(data["to"]);

          if (strcmp("null", to_c) == 0) { // se envia a todos
            printf("to es null\n");
            for (std::vector<User>::iterator user = users.begin(); user != users.end(); ++user) {
              json send_msg = send_message(username, msg);
              char *msg_c = to_char(send_msg);
              write((*user).id, msg_c, BUFFER_SIZE);
              delete[] msg_c;
            }
          } else { // solo usuario seleccionado
            // solo un usuario
            char *to_username = to_char(data["to"][0]);
            int to_socket = (*get_user_by_username(to_username, users)).id;
            json send_msg = send_message(username, msg);
            char *msg_c = to_char(send_msg);
            write(to_socket, msg_c, BUFFER_SIZE);
            delete[] msg_c;
          }

          delete[] msg;

        }
        else if (code == 3) { // get user or usersss
          json response, data, data_static;
          data_static = request["data"];
          response["code"] = 203;
          int s = 0;
          
          if (data_static.find("users") != data_static.end()) { // con users
            std::vector<string> users_vector = data_static["users"]; // lista de usuarios 
            for (std::vector<string>::iterator user_str = users_vector.begin() ; user_str != users_vector.end(); ++user_str) {
              char *username = str_to_char(*user_str);
              printf("Elemento: %s \n", username);
              User *current_user = get_user_by_username(username, users);
              s = 1;
              if (current_user != NULL) {
                printf("es nulo\n");
                json user_json = get_user_json(*current_user);
                data["users"].push_back(user_json);
              }
            }
            if (s == 0) {
              json all_users = get_all_users_json(users);
              data["users"] = all_users;
            }
            response["data"] = data["users"];
            char *response_str = to_char(response);
            write (sock, response_str, BUFFER_SIZE);
          }
          else { // no tiene users
            response["code"] = 203;

            json all_users = get_all_users_json(users);
            response["data"] = all_users;

            char *response_str = to_char(response);
            
            write (sock, response_str, BUFFER_SIZE);
          } 

        }
        else if (code == 4) { // change status
          json response, r_data;
          json data = request["data"];
          int id = data["user"];
          int new_status = data["new_status"];
          int val_stat = status_admitted(new_status);
          User *user = get_user_by_id(id, users); // user

          if (user != NULL && val_stat == 0) {
            user->status = new_status;
            // todo bien
            response = success_status();

            char *response_str = to_char(response);
            write(sock, response_str, BUFFER_SIZE);
            delete[] response_str;
          } else {
            // todo bien
            response = reject_status();

            char *response_str = to_char(response);
            write(sock, response_str, BUFFER_SIZE);
            delete[] response_str;
          }


        }

        if (result == NULL) 
          printf("NO\n");

      } else {
        printf("no es un json \n");
        // TODO -- write error json para que envie porfavor un json 
      }
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
  if(listen(socketfd, 10) == -1)
	printf("error en el listening");

    pthread_t connection_thread;

    int thread_status = pthread_create(&connection_thread, NULL, check_connections, (void *) &socketfd);  
    pthread_join(connection_thread, NULL);
    return 0;
}
