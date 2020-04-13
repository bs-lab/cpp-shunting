#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "parser.h"
using namespace std;


int main() {
  const string my_string = "A + BB *C + 10.0**3 + 7.6.5";
  cout << "card: " << my_string << endl;

  vector<TokenClass> tokens;
  tokenizer(my_string, tokens);
  cout << "\nTOKEN VALUE AND TYPE" << endl;
  for (uint i = 0; i < tokens.size(); i++) {
    cout  << "i = " << i << "\t" << tokens[i].svalue << "\t" << tokens[i].ttype << endl;
  }
}
