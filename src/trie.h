#ifndef TRIE_H
#define TRIE_H

#include <iostream>
#include <cstring>
#include <cctype>
#include <string>
using namespace std;

#include "aryheap.h"
#include "f_assert.h"
#include "exception.h"
#include "dlist.h"
#include "common.h"

const int MAX_WORD_LENGTH = 100;


class Trie {
private:
  bool _terminator;
  bool _allocated;
  Trie** _c;

  void allocate() {
    f_assert(_c=new Trie*[26]);
    for(int i=0;i<26;i++) _c[i]=NULL;
    _allocated=true;
  }
public:
  Trie(): _terminator(false),_allocated(false) {}
  ~Trie() {
    if(_allocated) {
      for(int i=0;i<26;i++) {
        if(_c[i]) delete _c[i];
      }
      delete[] _c;
    }
  }
  void clear() {delete[] _c; _c=NULL; _allocated=false;}
  bool has(const char word[]) const; //true if found
  bool put(const char word[]); //insert word, true if successful
  void words_length(Dlist<string>& list) const;
  void words_alpha(Dlist<string>& list) const;
  void words_alpha(Dlist<string>& list,char pre[MAX_WORD_LENGTH],int pos) const;


  void print(int level=0) const {
    if(_allocated)
    for(int i=0;i<26;i++) {
      if(_c[i]) {
        _c[i]->print(level+1);
        for(int j=0;j<level;j++) cout<<' ';
        cout<<char('a'+i);
        if(_terminator) cout<<".";
        cout<<endl;
      }
    }
  }


};

//class Trie;
/*
class Trie_node {
public:
	friend class Trie_base;
  friend class Trie;
private:
  //variables
  bool terminator;
  bool allocated;
	Trie_node** C;

  //methods
  Trie_node():terminator(0),allocated(0) {}
  ~Trie_node() {
    if(allocated) {
      for(int i=0;i<26;i++) if(C[i]) delete C[i];
      delete[] C;
    }
  }

  void allocate() {
	  f_assert(C = new Trie_node*[26]);
  	for(int i=0;i<26;i++) C[i]=0;
    allocated=true;
  }

};
*/

/*
//Trie_base: an "abstract" base class for Trie and Trie_subtrie
class Trie_base {
protected:
  Trie_node*& _root;
  Trie_base(Trie_node*& root):_root(root) {}
  Trie_base(Trie_base& t):_root(t._root) {}
  int _num_words;

public:
  int num_words() { return _num_words; }
  bool has(const char* word) const;
  void insert(const char* word);
  void clear() {
   if(_root) delete _root;
   _root = 0;
  }
  Trie_base get_prefix(const char* prefix);

  void get_by_length(Dlist<string>& list) const;
  void get_by_alpha(Dlist<string>& list) const;

};
*/
/*
class Trie_subtrie: public Trie_base {
friend class Trie;
private:
  string _prefix;
  Trie_subtrie(Trie_base& base, string prefix):
   Trie_base(base),
   _prefix(prefix) {}
public:
  string prefix() const { return _prefix; }
  bool has_suffix(char* word) const { return has(word); }
  
  Trie_subtrie(Trie_subtrie& rhs):_prefix(rhs._prefix),Trie_base(rhs) {}

  Trie_subtrie get_prefix(char* prefix) {
    return Trie_subtrie(Trie_base::get_prefix(prefix),_prefix+prefix);
  }


}; */

/*
class Trie: public Trie_base {
public:
  Trie():Trie_base(*(new Trie_node*(0))) {
    _num_words = 0;
  
  } //allocate mem for root reference
  ~Trie() {
    clear();
    delete &_root; //delete address of root reference
  }

  Trie_subtrie get_prefix(const char* prefix) {
    return Trie_subtrie(Trie_base::get_prefix(prefix),prefix);
  }

  //loads words into the trie
  void build(istream& in);
  void build(istream &in,ostream& out,int interval=10000,char delim='.');

  void write_simple_inorder(ostream& out) const;
  void  read_simple_inorder(istream& in );
  //void write_balanced_inorder(ostream& out,int segments) const;
};
*/


/*
class Trie {
public:
  //friend ostream& operator<<(ostream&,const WordTree&);
  //enum PrintOrder { alphabetical,longtoshort };

private:
  Trie_node* _root;
  //PrintOrder printOrder;
  //int printLevel;
  int _num_words;
public:
  //void setPrintOrder(PrintOrder p) { printOrder=p; }
  //void setPrintLevel(int i) { printLevel=i; }
  //void print(ostream&) const;
  int num_words() { return _num_words; }

  //void dft(void (*f)(const char* word));

  void insert(const char* word);

  Trie():_num_words(0) {
    f_assert(_root=new Trie_node);
    }
  //WordTree(WordTreeNode* r,int lvl=1):root(r) {}
  ~Trie() { delete _root; }
   bool has(const char* word);
   //Trie_node* get_prefix(char* prefix);

   //WordTree* scanBase(char* base); //create tree with given base

  void clear() {
    delete _root;
    _root = new Trie_node;
  }

  //Trie_subtrie get_subtrie(char* prefix); 


  //void printlong(ostream &out);

};
*/




#endif
