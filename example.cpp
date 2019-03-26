#include <nlohmann/json.hpp>
using namespace std;

using json = nlohmann::json;

int main(){

  json j;
  j["pi"] = 3.141;
  cout << j["yes"] <<'\n';
}
