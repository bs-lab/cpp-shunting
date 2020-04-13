#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "parser.h"
using namespace std;


static vector<string> operators = {"**", "^", "/", "*", "-", "+", "==",
                                    "<=", ">=", "<", ">", "<>", "!="};
static vector<int> oper_priority = {3, 3, 2, 2, 1, 1, 0,
                                    0, 0, 0, 0, 0, 0};


char peek_next_char(const string& card, const uint& curr_loc)
{
  if (curr_loc == card.length())
    return '\0';
  else
    return card[curr_loc];
}


int get_oper_index(const vector<string>& operators, const string& oper)
{
  for (uint i = 0; i < operators.size(); i++) {
    if (oper == operators[i])
      return i;
  }
  return -1;
}

int get_priority(const string& oper)
{
  // use enum for this maybe? something like a map/dict maybe?
  int indx = get_oper_index(operators, oper);
  return oper_priority[indx];
}


int tokenizer(const string& card, vector<TokenClass>& tokens)
{
  // tokenizes the input card
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

  // Clean up any instances where a plus or minus sign (unary) that should be assocated with
  //   a value was instead treated as a separate token.

  //for (uint t = 0; t < tokens.size() - 2; t++) {
  uint t = 0;
  //while (true) {
  while (t < tokens.size() - 2) {
    if (tokens[t].ttype == "operator" && tokens[t+1].ttype == "operator" &&
        (tokens[t+2].ttype == "number" || tokens[t+2].ttype == "variable")) {
      if (tokens[t+1].svalue == "-") {
        if (tokens[t+2].ttype == "number") {
          // change "... * - 31" to "... * -31"
          tokens[t+1].svalue = "-" + tokens[t+2].svalue;
          tokens[t+1].ttype = tokens[t+2].ttype;

          for (uint i = t+2; i < tokens.size()-1; i++)
            tokens[i] = tokens[i+1];
          tokens.pop_back();

        } else {
          TokenClass tempy("(", "left_paren");
          tokens.insert(tokens.begin()+t+1, tempy);

          tokens[t+2].svalue = "-1";
          tokens[t+2].ttype = "number";

          TokenClass tempy2("*", "operator");
          tokens.insert(tokens.begin()+t+3, tempy2);

          TokenClass tempy3(")", "right_paren");
          tokens.insert(tokens.begin()+t+5, tempy3);
        }
      } else if (tokens[t+1].svalue == "+") {
        // change "* + 31" to "* 31"
        for (uint i = t+1; i < tokens.size() -1; i++)
          tokens[i] = tokens[i+1];
        tokens.pop_back();
      } else {
        cout << "error in pattern" << endl;
        cout << "  at t:   " << tokens[t].svalue << endl;
        cout << "  at t+1: " << tokens[t+1].svalue << endl;
        cout << "  at t+2: " << tokens[t+2].svalue << endl;
      }
    }
    t++;
  }

  return 0;
}


int create_stack(const vector<TokenClass>& tokens, vector<string>& output_stack)
{
  // using Shunting -yard algorithm to convert list of tokens into Reverse Polish notation (RPN)
  vector<string> oper_stack;

  for (uint t = 0; t < tokens.size(); t++) {
    if (tokens[t].ttype == "number" || tokens[t].ttype == "variable") {
      output_stack.push_back(tokens[t].svalue);
    }

    if (tokens[t].ttype == "operator") {
      int priority = get_priority(tokens[t].svalue);
      while (oper_stack.size() > 0) {
        int top_priority = get_priority(oper_stack.back());
        if (priority >= top_priority || oper_stack.back() == "(") break;

        output_stack.push_back(oper_stack.back());
        oper_stack.pop_back();
      }
      oper_stack.push_back(tokens[t].svalue);
    }

    if (tokens[t].ttype == "left_paren") {
      oper_stack.push_back(tokens[t].svalue);
    }

    if (tokens[t].ttype == "right_paren") {
      if (oper_stack.size() == 0) {
        cout << "unexpected closing parentheses" << endl;
      }

      while (oper_stack.back() != "(") {
        output_stack.push_back(oper_stack.back());
        oper_stack.pop_back();
      }

      if (oper_stack.back() == "(") {
        oper_stack.pop_back();
      }
    }
  }

  // move remaining operation stack onto output stack
  while (oper_stack.size() > 0) {
    output_stack.push_back(oper_stack.back());
    oper_stack.pop_back();
  }

  return 0;
}
