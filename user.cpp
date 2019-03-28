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
    printf("%s = %s\n", username, u_name);
    if (strcmp(username, u_name) == 0) {
      return -1;
    }
  }
  return 0;
}