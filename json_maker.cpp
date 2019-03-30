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

using json = nlohmann::json;

// from json to char *
char *to_char(json js){
  string str = js.dump();
  char *c = new char[str.length() + 1];
  strcpy(c, str.c_str());
  return c;
}

// get char *
char *str_to_char(string str) {
  char *c = new char[str.length() + 1];
  strcpy(c, str.c_str());
  return c;
}

// get code char
int get_code(char *request) {
  json response = json::parse(request);
  return response["code"];
}

// get data char
json get_data(char *request) {
  json response = json::parse(request);
  return response["data"];
}

json send_connection(char* username){
  json response, data;
  response["code"] = 0;
  data["username"] = username;
  response["data"] = data;
  return response;
}

// accept connection
json accept_connection (int id, char* username, int status) {
  json response, data, user;
  response["code"] = 200;
  user["id"] = id;
  user["username"] = username;
  user["status"] = status;
  data["user"] = user;
  response["data"] = data;
  return response;
}

// fail connection
json reject_connection () {
  json response, data;
  response["code"] = 500;
  data["error_message"] = "Error en la conexión";
  return response;
}

// send message
json send_message (char *from, char* message) {
  json response, data;

  response["code"] = 201;
  data["from"] = from;
  data["message"] = message;
  response["data"] = data;

  return response;
}

//cange status
json success_status(){
  json response, data;
  response["code"] = 204;
  response["data"] = {};

  return response;	

}

json reject_status() {
  json response, data;
  response["code"] = 504;
  data["error_message"] = "No se pudo cambiar el status";
  response["data"] = data;
  return response;
}

json change_status(int user_id, int status){
  json response, data;
  response["code"] = 4;
  data["user"] = user_id;
  data["new_status"] = status;
  response["data"] = data;

  return response;
}

json obtener_usuarios(){
 json response, data;
  response["code"] = 3;
  response["data"] = data;

  return response;
}


json obtener_usuario(int id){
 json response, data;
  response["code"] = 3;
  data["user"] = id;
  response["data"] = data;

  return response;
}

json request_send(char* from,string message){
 json response,data;
  response["code"] = 1;
  data["from"] = from;
  data["message"] = message;
  response["data"] = data;
  return response;
}

