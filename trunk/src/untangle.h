#ifndef untangle_h
#define untangle_h

#include <cctype>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

#include "matrix.h"
#include "trie.h"
#include "dstack.h"
#include "aryheap.h"
#include "mcgi.h"
#include "exception.h"

const int MAX_BOARD_SIZE = 50;
const int FILE_BLOCK_READ_SIZE = 4096;

struct Untangle_exception: Exception {
  Untangle_exception(string s):Exception(s) {}
};

class Visited: public Matrix<bool> {
public:
  Visited(): Matrix<bool>() {}
  Visited(int x,int y):Matrix<bool>(x,y) { clear(); }
  bool ok(int,int);
};

class Board: public Matrix<char> {
public:
  Board(): Matrix<char>() {}
  Board(int x,int y):Matrix<char>(x,y) {}
  friend ostream& operator<<(ostream& out,const Board& board);
  friend istream& operator>>(istream& in,Board& board);
};

struct Letter {
  Letter* prev;  //used for storing paths for duplicates
  char l;
  int row,col;
  bool operator< (const Letter& r) {return l<r.l;}
  bool operator==(const Letter& r) {return (l==r.l && row==r.row && col==r.col && prev==r.prev);}
  Letter(char l0,int row0,int col0,Letter* p=0):l(l0),row(row0),col(col0),prev(p) {}
};



class Untangle {
  private:
    int _size,_mode,_min;
    
    Visited visited;
    Board board;
    ifstream dic;
    
    Trie _result;

    char lastprefix[MAX_WORD_LENGTH];
    bool scanToPrefix(const char* prefix);

    //prefix scanning
    char _word_reg[MAX_WORD_LENGTH];
    int _reg_pos;
    char _data_block[FILE_BLOCK_READ_SIZE];
    int _block_pos;
    int _block_actual;

  public:
    string get_letters();
    const Trie& result() {return _result;}

    int size() { return _size; }
    
    void print(ostream& out) { cout<<board; }

    void print_alpha(ostream&);
    void print_longest(ostream&);

    friend ostream& operator<<(ostream& out,const Untangle &s);

    int min() const { return _min; }
    void min(int);

    Untangle(const char* board,string dictionary,int min,int mode);
    ~Untangle() {}

    void mode(int m) {_mode=m;}
    
    void untangle();
    void rUntangle(AryHeap<Letter>& heap,char* word,char* letter);

    void restorePath(Letter* L,Stack<int>& stack);
    void removePath(Stack<int>& stack);
    //int num_words() { return _result.num_words(); }
  };



#endif
