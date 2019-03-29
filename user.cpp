#include <iostream>
#include <chrono>
#include <ctime>    
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
 
using namespace std;

class User {
  public:
    int id;
    char *username;
    int status;
    time_t last_connected;

  void init(int _id, char *_username) {
    id = _id;
    username = _username;
    status = 0;
    last_connected = std::time(0);
  }

  void update_time(){
    last_connected = std::time(0);
  }
};

int username_duplicate(char *username, vector<User> users) {
  for (std::vector<User>::iterator user = users.begin() ; user != users.end(); ++user) {
    User u = (*user);
    char* u_name = u.username;
    if (strcmp(username, u_name) == 0) {
      return -1;
    }
  }
  return 0;
}

//regresa 0 si el status es admisible
//regresa /1 de lo contrario

int status_admitted(int status){
    if(status == 0 || status == 1 || status == 2){
       return 0;
    }
    return -1;

}


User *get_user_by_id(int id, vector<User> users) {
  for (std::vector<User>::iterator user = users.begin() ; user != users.end(); ++user) {
    int u_id = (*user).id;
    if (id == u_id) {
      return &(*user);
    }
  }
  return NULL;
}

User *get_user_by_username(char *username, vector<User> users) {
  for (std::vector<User>::iterator user = users.begin() ; user != users.end(); ++user) {
    User u = (*user);
    char* u_name = u.username;
    if (strcmp(username, u_name) == 0) {
      return &(*user);
    }
  }
  return NULL;
}
