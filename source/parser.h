#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

//vector<string> operators(13);


struct TokenClass {
  string svalue;  // e.g. "12.345" or "var_name"
  string ttype;   // "number", "variable", "left_paren", "right_paren", "operator"
  TokenClass(string svalue, string ttype)
    :svalue(svalue), ttype(ttype) {
  }
};


char peek_next_char(const string& card, const uint& curr_loc);

int get_oper_index(const vector<string>& operators, const string& oper);

int get_priority(const string& oper);

int tokenizer(const string& card, vector<TokenClass>& tokens);

int create_stack(const vector<TokenClass>& tokens, vector<string>& output_queue);
