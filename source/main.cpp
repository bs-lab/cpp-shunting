#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "parser.h"
using namespace std;


int main()
{
  string card;
  // read equations from standard input
  while (getline(cin, card)) {
    if (card == "") continue;
    cout << "\ncard: " << card << endl;
    vector<TokenClass> tokens;
    vector<string> output_queue;

    tokenizer(card, tokens);
    create_stack(tokens, output_queue);

    cout << "tokens:";
    for (uint i = 0; i < tokens.size(); i++)
      cout << " " << tokens[i].svalue;
    cout << endl;

    //cout << "--- types are:" << endl;
    //for (uint i = 0; i < tokens.size(); i++) {
      //cout << " " << tokens[i].ttype;
    //}
    //cout << endl;

    cout << "rpn stack:";
    for (uint i = 0; i < output_queue.size(); i++)
      cout << " " << output_queue[i];
    cout << endl;
  }
}
