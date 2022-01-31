#include <iostream>

using namespace std;
int main(int argc, char* argv[]){
  for(string s; getline(cin, s); ){
    cout << s;
    for(int i=1; i <argc; i++){ 
      cout << " x " << argv[i];
    }
    cout << endl;
  }
  return 0;

}
