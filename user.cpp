#include <iostream>
#include <chrono>
#include <ctime>    
 
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
    int u_id = u.id;
    int u_status = u.status;
    int u_itime = u.last_connected;
    printf("El id del usuario es: %d", u_id);
    printf("El status del usuario es: %d", u_status);
    printf("El tiempo de coneccion usuario es: %d", u_itime);
    if (strcmp(username, u_name) == 0) {
      return -1;
    }
  }
  return 0;
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
