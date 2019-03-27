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