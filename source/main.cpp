#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;


struct TokenClass {
  string svalue;  // e.g. "12.345" or "var_name"
  string ttype;   // "number", "variable", "left_paren", "right_paren", "operator"
  TokenClass(string svalue, string ttype);
};

TokenClass::TokenClass(string sv, string tt)
  :svalue(sv), ttype(tt)
{
} 

char peek_next_char(const string& card, const uint& curr_loc) {
  if (curr_loc == card.length()) {
    return '\0';
  } else {
    return card[curr_loc];
  }
}


int get_oper_index(const vector<string>& operators, const string& oper) {
  for (uint i = 0; i < operators.size(); i++) {
    if (oper == operators[i]) {
      return i;
    }
  }
  return -1;
}


int tokenizer(const string& card, vector<TokenClass>& tokens) {
  vector<string> operators(13);
  operators[0] = "**";
  operators[1] = "^";
  operators[2] = "/";
  operators[3] = "*";
  operators[4] = "-";
  operators[5] = "+";
  operators[6] = "==";
  operators[7] = "<=";
  operators[8] = ">=";
  operators[9] = "<";
  operators[10] = ">";
  operators[11] = "<>";
  operators[12] = "!=";

  char curr_char, next_char;
  uint c = 0;
  while (c < card.length()) {
    curr_char = card[c];
    c += 1;
    if (curr_char == ' ')  continue;

    if (curr_char == '(') {
      TokenClass temp(string(1, curr_char), "left_paren");
      tokens.push_back(temp);
      continue;
    } else if (curr_char == ')') {
      TokenClass temp(string(1, curr_char), "right_paren");
      tokens.push_back(temp);
      continue;
    }

    // check if this character plus the next character are a two-character operator
    next_char = peek_next_char(card, c);
    string twochars;
    twochars += curr_char;
    twochars += next_char;

    if (next_char != '\0' && next_char != ' ') {       // neither null nor space
      int loc = get_oper_index(operators, twochars);
      if (loc >= 0) {
        TokenClass temp(twochars, "operator");
        tokens.push_back(temp);
        ++c;
        continue;
      }
    }

    // check if this character is a single-character operator
    int loc = get_oper_index(operators, string(1, curr_char));
    if (loc >= 0) {
      TokenClass temp(string(1, curr_char), "operator");
      tokens.push_back(temp);
      continue;
    }

    // check for alphanumerics (numeric values or variable names)
    if (isalnum(curr_char) || curr_char == '.') {
      string svalue, ttype;
      if (isdigit(curr_char) || curr_char == '.') {
        ttype = "number";
      } else {
        ttype = "variable";
      }
      svalue += curr_char;

      // read until next non-alphanumeric value
      while (true) {
        next_char = peek_next_char(card, c);
        if (next_char == '\0')  break;  // found end-of-line

        if (isalnum(next_char) || next_char == '.') {
          ++c;
          svalue += next_char;
        } else { 
          break;   // found end of number or variable name
        }
      }

      // push result onto vector
      TokenClass temp(svalue, ttype);
      tokens.push_back(temp);
    }
  }

  //cout << "\n\nTOKENS\n";
  //cout << "size=" << tokens.size() << endl;
  //for (uint i = 0; i < tokens.size(); i++) {
    //cout  << "i = " << i << "\t" << tokens[i].svalue << "\t" << tokens[i].ttype << endl;
  //}

  return 0;
}


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
