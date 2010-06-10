// Driver for Tangleword Solver
// (c) Michael Hecht
// http://www.mh-z.com

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

#include "command.h"
#include "untangle.h"
#include "dlist.h"

char LOGFILE[] = "solve_log.txt";
char LOCKFILE[] = "solve_log.txt.lock";
const int NUM_ENTRIES = 100;

void print_html(string board,int min,int size);

void tabular_result_output(const Dlist<string>& words,int cols);

int main(int argc,char* argv[]) {

  Command cmd(argc,argv);

  try {    

    string dic, board;
    int cols, min, mode;

    try {
      board = cmd.index().get("board");
    } catch(...) {
      cout<<"No board given."<<endl;
      throw;
    }

    try {
      min = atoi(cmd.index().get("min").c_str());
    } catch(...) {
      cout<<"Setting minimum word length to 4"<<endl;
      min=4;
    }
    
    try {
      dic = cmd.index().get("dic");
      for (int i = 0; i < (int)dic.size(); i++) {
        if (!isalpha(dic[i])) {
          dic[i]='_';
        }
      }
    } catch(...) {
      cout<<"Using default dictionary."<<endl;
      dic = "words";
    }

    try {
      mode = atoi(cmd.index().get("mode").c_str());
      if(mode) mode=1;
    }
    catch(...) {cout<<"Using default mode 0"<<endl; mode=0;}

    try {
      cols = atoi(cmd.index().get("cols").c_str());
      if(cols<2 || cols > 100) cols = 10;
    }
    catch(...) {cout<<"Using default output column count."<<endl; cols=10;}


    Untangle s(board.c_str(), dic+".trie", min, mode);

    cout<<"<p>Board size: "<<s.size()<<"<br>\n";
    cout<<"Minimum word length: "<<min<<"<br>\n";
    cout<<"Solve mode: "<<mode<<"</p>\n";



    cout<<"<p>Board:\n";
    print_html(s.get_letters(),min,s.size());
    cout<<"</p>\n"<<endl;

    cout<<"<p>Solving...<br>\n"<<endl;
    s.untangle();
    cout<<"<p>All done, sorting and displaying the result...</p>"<<endl;

    Dlist<string> words;

    cout<<"<h2>Alphabetically</h2>\n";   
    s.result().words_alpha(words);

    tabular_result_output(words,cols);

    words.clear();
    cout<<"<h2>Longest-first</h2>\n";
    s.result().words_length(words);

    tabular_result_output(words,cols);

    ofstream log("solve_log.txt");
    if(log.fail()) throw "Could not open logfile for writing.";

    log<<s.min()<<'\n'<<dic<<'\n'<<s.get_letters()<<'\n';
    log.close();

    cout<<"<p>"<</*s.num_words()<<" words found. */"Solution complete.</p>\n";
    }
  catch(string e) {
    cout<<e<<endl;
    cout<<"Error in getting program parameters."<<endl;
  }
  catch(char* e) {
    cout<<e<<endl;
  }
  catch(...) {
    cout<<"Unspecified error occurred. Please e-mail the author with the parameters that caused this condition."<<endl;
  }
  cout<<"<p>Boggle/Tangleword solver by Michael Hecht (c) 1999-2004</p>\n";
  return 0;  


}



void print_html(string board,int min,int size) {
  cout<<"<table border=\"1\">"<<endl;
  int i,j;
  for(i=0;i<size;i++) {
    cout<<" <tr>"<<endl;
    for(j=0;j<size;j++) {
      cout<<"  <td align=\"center\"><b>"<<board[i*size + j]<<"</b></td>"<<endl;
    }
    cout<<" </tr>"<<endl;
  }
  cout<<"</table>"<<endl;
}


void tabular_result_output(const Dlist<string>& words,int cols) {
  cout<<"<table>\n<tr>\n";

  int rowpos=0;
  for(int i=0;i<words.size();i++) {
    cout<<"<td>"<<words[i]<<"</td>\n";    
    if(!(++rowpos%cols) && (i+1 != words.size())) cout<<"</tr>\n<tr>\n";
  }

  cout<<"</tr>\n</table>\n";
}
