#ifndef TRIE_CPP
#define TRIE_CPP

#include "trie.h"
#include "aryheap.h"

bool Trie::put(const char word[]) {
  char pos;
  Trie* cur = this;
  while(*word) {
    if(!cur->_allocated) cur->allocate();
    pos = tolower(*word)-'a'; //get array position for letter
    if(pos >= 26) throw "Trie::put: Letter is out of bounds.";
    if(!cur->_c[pos]) cur = (cur->_c[pos] = new Trie());
    else cur=cur->_c[pos];
    //cout<<"Created new trie; allocated="<<cur->_allocated<<" terminator="<<cur->_terminator<<endl;
    word++;
  }

  if(cur->_terminator) return false;
  cur->_terminator = true;
  return true;
}

bool Trie::has(const char word[]) const {
  char pos;
  const Trie* cur = this;
  while(*word) {
    if(!cur->_allocated) return false;
    pos = tolower(*word)-'a';
    if(pos >= 26) throw "Trie::has: Letter is out of bounds";
    if(!(cur = cur->_c[pos])) return false;
    word++;
  }
  return cur->_terminator;
}


/*
void Trie_base::insert(const char* word) {
  unsigned char pos;
  if(!_root) _root = new Trie_node();
  Trie_node* cur = _root;
  while(*word) {
    if(!cur->allocated) cur->allocate();
    pos = tolower(*word)-'a'; //get array position for letter
    if(pos >= 26) throw string("Letter is out of bounds.");
    if(!cur->C[pos]) cur->C[pos] = new Trie_node;
    cur = cur->C[pos];
    word++;
  }
  if(cur->terminator) {
    cout<<"Duplicate insertion."<<endl;
    return;
  }
  cur->terminator = true;
  _num_words++;
  return;
}
*/
/*
//---check for word from base---
bool Trie_base::has(const char* word) const {

}
*//*
Trie_base Trie_base::get_prefix(const char* prefix) {
  cout<<"starting get_prefix"<<endl;
  unsigned char pos;
  Trie_node** cur = &_root;

  while(*prefix) {
    if(!(*cur)) throw string("Word was not found.");
    if(!(*cur)->allocated) throw string("Word was not found.");
    pos = tolower(*prefix)-'a';
    cout<<"letter="<<((char)(pos+'a'))<<' '<<flush;
    if(pos >= 26) throw string("Character out of bounds.");
    cur = &((*cur)->C[pos]);
    prefix++;
  }
  return Trie_base(*cur);
}*/

/*
void Trie::build(istream& in) {
  char word[MAX_WORD_LENGTH];
  for(;;) {
    in>>word;
    if(in.eof()) break;
    insert(word);
  }
}
*/

/*
void Trie::build(istream &in,ostream& out,int interval,char delim) {
  char word[MAX_WORD_LENGTH];
  for(int i=0;;) {
    in>>word;
    if(in.eof()) break;
    insert(word);
    if(!(++i%interval)) {
      out.put(delim);
      out.flush();
    }
  }
}
*/

/*
void Trie::write_simple_inorder(ostream& out) const {
  Trie_node* ptr_stack[MAX_WORD_LENGTH];
  int ptr_stack_top = 0;
  char pos_stack[MAX_WORD_LENGTH];
  int pos_stack_top = 0;

  Trie_node* cur = _root;
  if(!cur) return;

  int pos = 0;
  int num_back = 0;

  for(;;) {
    while(pos<26) {
      //cerr<<"("<<pos<<")";
      if(cur->C[pos]) {
        ptr_stack[ptr_stack_top++] = cur;
        pos_stack[pos_stack_top++] = pos;
        cur = cur->C[pos];

        if(num_back) {
          out.put(num_back+'z');
          num_back=0;
        }
        if(cur->terminator) out.put(pos+'A');
        else out.put(pos+'a');
        pos = 0;
        if(!cur->allocated) break;
      }
      else pos++;
      
    } //end while
   
    if(!ptr_stack_top) break;
    num_back++;
    cur = ptr_stack[--ptr_stack_top];
    pos = pos_stack[--pos_stack_top]+1;
  }
}
*/

/*
void Trie::read_simple_inorder(istream& in) {
  char ch;
  Trie_node* ptr_stack[MAX_WORD_LENGTH];
  int ptr_stack_top = 0;
  char pos_stack[MAX_WORD_LENGTH];
  int pos_stack_top = 0;

  if(!_root) _root = new Trie_node();
  Trie_node* cur = _root;

  while(in.get(ch) && in) {



  }
}

*/
  


void Trie::words_alpha(Dlist<string>& list,char pre[MAX_WORD_LENGTH],int pos) const {
  if(_terminator) {pre[pos]=0; list.insert(string(pre));}
  if(_allocated) {
    for(int i=0;i<26;i++) {
      if(_c[i]) {
        pre[pos]=i+'a';
        _c[i]->words_alpha(list,pre,pos+1);
      }
    }
  }
}

void Trie::words_alpha(Dlist<string>& list) const {
  char pre[MAX_WORD_LENGTH];
  words_alpha(list,pre,0);
  list.reverse();
  }
/*
  const Trie* ptr_stack[MAX_WORD_LENGTH];
  int ptr_stack_top = 0;
  char word[MAX_WORD_LENGTH];
  int word_top = 0;

  const Trie* cur(this); 

  int pos=0;

  for(;;) {


    while(cur && pos<26) {
      cout<<"pos="<<pos<<endl;
      if(!cur->_allocated) break;
      

      if(cur->_c[pos]) {
        ptr_stack[ptr_stack_top++] = cur;
        word[word_top++] = pos+'a';
        cur = cur->_c[pos];

        if(cur->_terminator) {
          word[word_top]=NULL;
          cout<<"="<<word<<endl;
          list.insert(string(word));
        }
        pos = 0;
        for(int m=0;m<word_top;m++) cout<<word[m];
        cout<<endl;
      }
      else pos++;
    } //end while
   cout<<"<--"<<endl;
    if(ptr_stack_top=0) break;
    cur = ptr_stack[--ptr_stack_top];
    pos = word[--word_top]+1-'a';
   cout<<"---"<<endl;

  }
}
*/

void Trie::words_length(Dlist<string>& list) const {
  //should be reimplemented with a queue and BFT of the trie

  Dlist<string> temp;
  words_alpha(temp);

  Dlist<string> buckets[MAX_WORD_LENGTH];

  int i;
  for(i=0;i<temp.size();i++) {
    buckets[temp[i].size()].insert(temp[i]);
  }

  int j;
  for(i=MAX_WORD_LENGTH-1;i>=0;i--) {
    for(j=buckets[i].size()-1;j>=0;j--) {
      list.insert(buckets[i][j]);
    }
  }
  list.reverse();
}








      






/*
Trie_node* Trie::get_prefix(char* prefix) {
  unsigned char pos;
  Trie_node* cur = _root;
  while(*prefix) {
    pos = tolower(*prefix)-'a';
    if(pos >= 26) throw Trie_bounds_exception();
    cur = root->C[pos];
    if(!cur) throw Trie_notfound_exception();
  }
  return cur;
}
*/

//---print entire tree alphabetically---
/*
ostream& operator<<(ostream& out,const WordTree& w) {
  w.print(out);
  return out;
  }

void WordTree::print(ostream& out) const {
  char word[64];
  if(printOrder==alphabetical) {
    root->printAlphabetical(out,word,word,-printLevel);
    }
  else if(printOrder==longtoshort) {
    AryHeap<WordTreeItem> words(numWords);
    root->printLongtoshort(words,word,word,-printLevel);

    WordTreeItem* c;
    
    while(words.peek()) {
      c=words.remove();
      out<<c->word<<' ';
      delete[] c->word;
      }
    }
  }


void WordTreeNode::printAlphabetical(ostream& out,char* base,char* here,int lvl) const {
  if(terminator && lvl>=0) {*here=0; out<<base<<' ';}
  if(!allocated) return;
  for(int i=0;i<26;i++) {
    if(C[i]) {
      *here=(i+'a');
      C[i]->printAlphabetical(out,base,here+1,lvl+1);
      }
    }
  }

void WordTreeNode::printLongtoshort(AryHeap<WordTreeItem>& words,char* base,char* here,int lvl) const {
  if(terminator && lvl>=0) {
    *here=0;
    int size=strlen(base);
    char* w=new char[size+1];
    strcpy(w,base);
    words.insert(new WordTreeItem(w,size));
    }
  if(!allocated) return;
  for(int i=0;i<26;i++) {
    if(C[i]) {
      *here=(i+'a');
      C[i]->printLongtoshort(words,base,here+1,lvl+1);
      }
    }
  }
*/
  
  /*
void WordTreeNode::dft(char* base,char* here,int lvl,void f(const char*)) const {
  if(terminator && lvl>=0) {
    *here=0;
    f(base);
    }
  if(!allocated) return;
  for(int i=0;i<26;i++) {
    if(C[i]) {
      *here=(i+'a');
      C[i]->dft(base,here+1,lvl+1,f);
      }
    }
  }

void WordTree::dft(void (*f)(const char* word) ) {
  }
*/
/*

void WordTree::printlong(ostream &out) {
  char base[64];
  PtrQueue<WordTreeNode*> q;
  root->printlong(out,base,base,-minlevel,0,q);
  }
*/

/*
void WordTreeNode::printlong(ostream& out,char* base,char* here,int lvl,int height,PtrQueue<WordTreeNode*>& q) {
  if(terminator && lvl>=0) {
    char* word=new char[height];
    }
  if(!allocated) return;
  for(int i=0;i<26;i++) {
    if(C[i]) {
      *here=(i+'a');
      C[i]->printfull(out,base,here+1,lvl+1);
      }
    }
  }
*/

//---return pointer to subtree with given base path
/*
WordTree* WordTree::scanBase(char* base) {
  WordTreeNode* ptr=root;
  if(!ptr) return 0;
  while(*base && ptr->allocated && ptr->C[*(base-'A')]) {
    ptr=ptr->C[*((base++)-'A')];
    }
  return new WordTree(ptr);
  }
*/
#endif
