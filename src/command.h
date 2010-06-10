#ifndef COMMAND_H
#define COMMAND_H

#include <cstring>
#include <string>
#include <cstdio>
using namespace std;

#include "common.h"

class Command {
private:
  int _argc;
  char** _argv;

  Textmap _index;

public:
  Textmap& index() {return _index;}
  Command(int argc,char** argv):_argc(argc),_argv(argv) {
    int i,len,j;
    char temp[100];

  _index.insert("_filename",argv[0]);

  for(i=1;i<_argc;i++) {
      len=strlen(argv[i]);
      for(j=0;j<len && argv[i][j]!='=';++j);
      argv[i][j]=NULL;      
      //if(j==len) _index.insert(itoa(i,temp,10),argv[i]);
      _index.insert(argv[i],argv[i]+j+1);
    }
  } //end constructor


};

#endif
