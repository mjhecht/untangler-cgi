#ifndef MCGI_H
#define MCGI_H

#include <cstdlib>
#include <fstream>
#include <string>
#include <ctime>
#include <string>
#include <cstring>
#include <iostream>
using namespace std;

#include "common.h"

class Mcgi {
private:
  Textmap _query;
  
  //Btree<string,char*> _name_index;
  //Dlist<string> _pos_index;

  //char* _cgi_data;
  //int _num_vars;

  void parse(char* line);
  char decode(char* hex);

  //char* getword(char *line,char stop);

  static ostream* _out;

  static void print_var_traverser(const string& name,char*& data) {
    cout<<name<<" = "<<data<<endl;
  }


public:
  Mcgi();

  static void print_header() {cout<<"Content-type: text/html\n"<<endl;}
  
  //int   num_vars() const { return _num_vars; }
  //string name(int pos) { return _pos_index[pos]; }
  /*
  const char* value(int pos) {
    try {
      return _name_index.get(_pos_index[pos]);
    }
    catch(Btree_notfound_x) {
      throw Mcgi_exception("Variable not found");
    }
  }
  */

  string value(string name) {
    try {return _query.get(name);}
    catch(string e) {
      cout<<e<<endl;
      throw string("Could not look up CGI variable '") + name + "'.";
    }
    catch(...) {
      throw string("Could not look up CGI variable '") + name + "'.";
    }
  }
  
  int int_value(const char* name) {
    try {return atoi(_query.get(name).c_str());}
    catch(string e) {
      cout<<e<<endl;
      throw string("Could not look up int value of CGI variable '") + name + "'.";
    }
    catch(...) {
      throw string("Could not look up int value of CGI variable '") + name + "'.";
    }
  }
  void echo_file(char* filename);

  void print_vars(ostream& out) {
    out<<"Printing CGI variables"<<endl;
    _query.traverse(Textmap::print_pair);
    out<<endl;
    out<<"Done printing CGI variables"<<endl;
  }
  
};


#endif
