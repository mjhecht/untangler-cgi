#ifndef UNTANGLE_CPP
#define UNTANGLE_CPP

#include "untangle.h"

//    Untangle(int s):size(s),visited(s,s),board(s,s),
//      printAsFound(false) {}

Untangle::Untangle(const char* letters,string dictionary,int min,int mode) {
  _mode=mode;

  if(min<1) throw "Minimum word length for solution cannot be less than 1.";
  _min=min;
  //open dictionary
  dic.open(dictionary.c_str());
  if(!dic) throw "Dictionary file not found.";
  //read letters
  try { //...opened dictionary file
    lastprefix[0]=0;  //reset last prefix=
    if(!letters) throw "In parsing board, board is null.";
    int max_size=strlen(letters);
    char* valid_letters = new char[max_size+1];
    assert(valid_letters);
    try {
      int i,j,k;
      for(i=0,k=0;i<max_size;i++) {
        if(isalpha(letters[i])) valid_letters[k++]=tolower(letters[i]);
      }
      if(k<1) throw "In parsing board, too few letters.";
      switch(mode) {
      case 0:
        _size = int(floor(sqrt(double(k))));
        board.resize(_size,_size);
        board.clear();
        visited.resize(_size,_size);
        visited.clear();
        for(i=0,k=0;i<_size;i++) {
          for(j=0;j<_size;j++) {
            board[i][j]=valid_letters[k++];
          }
        }
        break;
      case 1:
        if(k<3) throw "In parsing board, too few letters";
        _size = 2*(int)floor(sqrt(double(k/3)));
        board.resize(_size,_size);
        board.clear();
        visited.resize(_size,_size);
        visited.clear();
        for(i=0,k=0;i<_size/2;i++) {
          for(j=0;j<_size/2;j++) {
            board[i][j]=valid_letters[k++];
          }
        }
        for(;i<_size;i++) {
          for(j=0;j<_size;j++) board[i][j]=valid_letters[k++];
        }
        for(i=0;i<_size/2;i++) for(j=_size/2;j<_size;j++) {
          board[i][j]='.';
        }

        break;
      default:
        throw "Unknown solver mode";
      }
    }
    catch(...) {
      delete[] valid_letters;
      throw;
    }
    delete[] valid_letters;
  }
  catch(...) {
    dic.close(); //release resource
    throw;
  }

  _reg_pos = 0;
  _block_pos = 0;
  _block_actual = 0;

}



bool Visited::ok(int row,int col) {
  return is_in(row,col)?(!(*this)[row][col]):false;
  }


ostream& operator<<(ostream& out,const Board& board) {
  for(int i=0;i<board.rows();i++) {
    for(int j=0;j<board.cols();j++) {
      out<<board.data[i][j]<<' ';
      }
    out<<endl;
    }
  return out;
  }


ostream& operator<<(ostream& out,const Untangle &s) {
  out<<s.board;
  return out;
  }


bool Untangle::scanToPrefix(const char* prefix) {
  //assumes a dictionary in a specific format; no validity checking

  //int i;
  unsigned char c;
  int length = strlen(prefix);

  int matched=0;

  for(;;) {
    if(_reg_pos>matched && tolower(_word_reg[matched]) == prefix[matched]) {
      matched++;
      //cout<<"matched="<<matched<<endl;

      if(!prefix[matched]) {        
        if(length>=_min && _word_reg[matched-1]<'a') {
          //cout<<"* Is a word: "<<prefix<<endl;
          _result.put(prefix);
        }
        //cout<<"- matched full length of prefix"<<endl;
        return true;
      }
    }
    else if(_reg_pos>matched && tolower(_word_reg[matched]) > prefix[matched]) {
      //cout<<"PASSED"<<endl;
      return false;
    }
    else {
      //cout<<"scanning..."<<endl;
      do {
        //cout<<" _word_reg=";
        //for(i=0;i<_reg_pos;i++) cout<<_word_reg[i];
        //cout<<".\n";

        if(_block_pos >= _block_actual) {
          if(!dic) return false; //finished dictionary
          
          //cout<<"reading next block from file"<<endl;
          dic.read(_data_block,FILE_BLOCK_READ_SIZE);
          _block_actual = dic.gcount();
          
          //cout<<"read "<<_block_actual<<" characters"<<endl;
          _block_pos = 0;
          if(!_block_actual) return false; //no data was read
        }
        c = _data_block[_block_pos++];
        //cout<<"c="<<c<<" c-z="<<(c-'z')<<endl;

        if(c > 'z') {
          _reg_pos -= (c-'z');
          //cout<<"-back="<<int(c-('z'+1))<<endl;
          if(_reg_pos < matched) return false;
        }
        else _word_reg[_reg_pos++] = c;
        //cout<<"_reg_pos before loop = "<<_reg_pos<<endl;
      } while((_reg_pos-1) > matched);
    } //end last else
  }

}




void Untangle::restorePath(Letter* L,Stack<int>& stack) {
  for(;L;L=L->prev) {
    visited[L->row][L->col]=1;
    stack.push(L->row);
    stack.push(L->col);
    }
  }

void Untangle::removePath(Stack<int>& stack) {
  int row,col;
  while(stack.inuse()) {
    col=stack.pop();
    row=stack.pop();
	  visited[row][col]=0;
    }
  }


void Untangle::untangle() {
  AryHeap<Letter> heap(_size*_size);
  for(int row=0;row<_size;row++) for(int col=0;col<_size;col++)
    heap.insert(new Letter(board[row][col],row,col));

  char word[MAX_WORD_LENGTH];
  rUntangle(heap,word,word);
  }

void Untangle::rUntangle(AryHeap<Letter>& heap,char* word,char* letter) {
  AryHeap<Letter> nextheap(heap.used()*8);
  Stack<int> stack;
  Stack<Letter*> memstack;
  int row,col,xrow,xcol;
  Letter* let=heap.remove();
  Letter* p;
  while(let) {
    *letter=let->l;
    *(letter+1)=0;

    if(scanToPrefix(word)) {
      do {
        row=let->row;
        col=let->col;

        if(let->l=='q') {
          p=new Letter('u',row,col,let);
          memstack.push(p);
          nextheap.insert(p);
        }
        restorePath(let,stack);  //unfold path of current letter
        visited[row][col]=1;         //mask current square

        
        switch(_mode) {
        case 0:
          for(xrow=row-1;xrow<=row+1;xrow++) {
            for(xcol=col-1;xcol<=col+1;xcol++) {
              if(visited.ok(xrow,xcol)) {
                p=new Letter(board[xrow][xcol],xrow,xcol,let);
                memstack.push(p);    //keep track of allocations
                nextheap.insert(p);  //queue letter
              }
            }
          }
          break;
        case 1:
          {
            int newr,newc;
            for(xrow=row-1;xrow<=row+1;xrow++) {
              for(xcol=col-1;xcol<=col+1;xcol++) {
                
                if(xrow<_size/2-1 && xcol==_size/2) {
                  if(visited.ok(newr=_size/2,newc=(_size-1)-xrow)) {
                    p=new Letter(board[newr][newc],newr,newc,let);
                    memstack.push(p);    //keep track of allocations
                    nextheap.insert(p);  //queue letter
                  }
                }
                else if(xrow==_size/2-1 && xcol>_size/2) {
                  if(visited.ok(newr=(_size-1)-xcol,newc=_size/2-1)) {
                    p=new Letter(board[newr][newc],newr,newc,let);
                    memstack.push(p);    //keep track of allocations
                    nextheap.insert(p);  //queue letter
                  }
                }
                else {
                  if(visited.ok(xrow,xcol)) {
                    p=new Letter(board[xrow][xcol],xrow,xcol,let);
                    memstack.push(p);    //keep track of allocations
                    nextheap.insert(p);  //queue letter
                  }
                }
              }
            }
            break;
          }
        } //end switch

        visited[row][col]=0; //unmask current square
        removePath(stack);

      } while((let=heap.remove()) && let->l==*letter);
      rUntangle(nextheap,word,letter+1);  //recur when no duplicates for current letter
    }
    else while((let=heap.remove()) && let->l==*letter) {}
  }
  while(memstack.inuse()) delete memstack.pop(); //deallocate
}


void Untangle::min(int i) {
  if(i<1) throw "Print level cannot be less than 1.";
  _min=i;
//  result.setPrintLevel(i);
  }

/*
void Untangle::print_longest(ostream& out) {
//  result.setPrintOrder(result.longtoshort);
 // out<<result;
  }


void Untangle::print_alpha(ostream& out) {
//  result.setPrintOrder(result.alphabetical);
//  out<<result;
  }
*/


string Untangle::get_letters() {
  string temp(_size*_size,' ');
  int i,j;
  for(i=0;i<_size;i++) for(j=0;j<_size;j++) {
    temp[i*_size + j] = board[i][j];
  }
  return temp;
}

#endif
