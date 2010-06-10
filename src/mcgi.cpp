#ifndef MCGI_CPP
#define MCGI_CPP

#include "mcgi.h"

const int MAX_DATA_SIZE=1024*16;
const int BLOCK_SIZE=1024;

Mcgi::Mcgi() {
  int size;

  char* request_method=getenv("REQUEST_METHOD");
  if(!request_method) {
    throw string("Environment variable REQUEST_METHOD was not found.");
  }



  /*
    if(!strcmp(request_method,"POST")) {
      content_length=getenv("CONTENT_LENGTH");
      if(!content_length) throw Mcgi_exception("Environment variable CONTENT_LENGTH was not found.");

      size=atoi(content_length);
      if(size > MAX_DATA_SIZE) throw Mcgi_exception("CONTENT_LENGTH is larger than the maximum");
      if(size<0) throw string("CONTENT_LENGTH is less than zero");
      if(size==0) return; //empty query string

      f_assert(_cgi_data=new char[size+1]);
      cin.get(_cgi_data,size);
    } //end POST handler
*/
    /*else*/
    
  if(!strcmp(request_method,"GET")) {
    char* pointer=getenv("QUERY_STRING");
    if(!pointer) throw string("No query string found.");
    
    size=strlen(pointer);
    if(size==0) throw string("Query string size is 0.");
    if(size>MAX_DATA_SIZE) throw string("Size of query string is too large.");

    //process GET data:
    int i=0;

    char* name_start;
    char* value_start;
    int var_count=0;

    name_start=strtok(pointer,"=");
    value_start=strtok(NULL,"&");
    while(name_start && value_start) {
      parse(name_start);
      parse(value_start);
      _query.insert(name_start,value_start);
      //_pos_index.insert(string(name_start));
      if(name_start=strtok(NULL,"=")) value_start=strtok(NULL,"&");
    }
  } //end GET mode if
  else throw string("Unhandled document request mode: '") + request_method + "'.";
}

char Mcgi::decode(char* hex) {
  char digit;
  digit= (hex[0] >= 'A' ? ((hex[0] & 0xDF) - 'A')+10 : (hex[0]-'0'));
  digit *= 16;
  digit += (hex[1] >= 'A' ? ((hex[1] & 0xDF) - 'A')+10 : (hex[1]-'0'));
  return(digit);
}

void Mcgi::parse(char *line) {
  int x,y;
  for(x=0,y=0;line[y];++x,++y) {
    if((line[x]=line[y]) == '%') {
      line[x]=decode(line+y+1);
      y+=2;
    }
  }
  line[x]=NULL;
}

void Mcgi::echo_file(char* filename) {
  ifstream in(filename);
  if(!in) throw string("File to echo was not found");
  char* block=new char[BLOCK_SIZE];
  while(in.read(block,BLOCK_SIZE)) {
    cout.write(block,in.gcount());
  }
  in.close();
}

#endif
