/*
class Btree
(c) 2001 Michael Hecht
*/

#ifndef BTREE_H
#define BTREE_H

const int STACK_SIZE=1000; //supports a tree of size 2^STACK_SIZE


//exceptions
class Btree_x {};
class Btree_duplicate_x: Btree_x {};
class Btree_notfound_x: Btree_x {};


//forward declaration of Btree
template<class K,class D> class Btree;

template<class K,class D> class Btree_node {
  friend class Btree<K,D>;
private:
  inline void lbalance(Btree_node*&);
  inline void rbalance(Btree_node*&);

protected:
  K key;
  D data;
  Btree_node* c1,* c2;
  char bal;

  int check();

  Btree_node(const K& newKey,const D& newData,Btree_node* C1=0,Btree_node* C2=0,int balance=0):
    key(newKey),data(newData),c1(C1),c2(C2),bal(balance) {
  }


  ~Btree_node() {cout<<"*"<<flush; if(c1) delete c1; if(c2) delete c2;}

  void traverse(void f(const K& key,D& data));
  void traverse(void f(const K& key)) const;
  void traverse(void f(D& data));
  
};


template<class K,class D> class Btree {

  protected:
    Btree_node<K,D>* root;
  
  public:
    enum action {REPLACE, EXCEPTION, IGNORE};

    action onInsertDuplicate;
    action onRemoveNotFound;
    
    Btree():
      root(0),
      onInsertDuplicate(EXCEPTION),
      onRemoveNotFound(EXCEPTION)
      {}

    void insert(const K& newKey,const D& newData);
    void remove(const K& oldKey);
    D* remove(const K& oldKey,D& retData);

    bool has(const K& key) const;
    
    D* get(const K& key,D& retData);
    D& get(const K& key) const;
    D* pop_any(D& retData);
    D* pop_min(D& retData);
    D* pop_max(D& retData);
    D* get_any(D& retData) const;

    D& operator[](const K& key) { return lookup(key); }

    ~Btree() { if(root) delete root;}

    void clear () { if(root) {delete root; root=0;} }
    bool is_empty() { return root?false:true; }

    void traverse(void f(const K& key,D& data)) { if(root) root->traverse(f); }
    void traverse(void f(const K& key)) { if(root) root->traverse(f); }
    void traverse(void f(D& data)) { if(root) root->traverse(f); }

    //traversers
    void copyto(const K& key,D& data) {
      insert(key,data);
    }


  };





template<class K,class D> inline
void Btree<K,D>::insert(const K& newKey,const D& newData) {
  Btree_node<K,D>** pStack[STACK_SIZE]; bool dStack[STACK_SIZE]; short stackTop=-1;

  Btree_node<K,D>** cur=&root;

  while(*cur) {
    if(newKey<(*cur)->key) {
      pStack[++stackTop]=cur;
      dStack[stackTop]=0; //0=left
      cur=&(*cur)->c1;
      }
    else if(newKey>(*cur)->key) {
      pStack[++stackTop]=cur;
      dStack[stackTop]=1; //1=right
      cur=&(*cur)->c2;
      }
    else {
      if(onInsertDuplicate==REPLACE) {
        (*cur)->data=newData;
        return;
        }
      else throw "Duplicate insertion";
      }
    }
  (*cur)=new Btree_node<K,D>(newKey,newData);

  bool dir;

  while(stackTop>=0) {
    cur=pStack[stackTop];
    dir=dStack[stackTop--];
    
    if(dir) { //right path
      if((++(*cur)->bal) == 2) {
        (*cur)->lbalance(*cur);
        break;
        }
      }
    else { //left path
      if((--(*cur)->bal) == -2) {
        (*cur)->rbalance(*cur);
        break;
        }
      }
    if(!(*cur)->bal) break;
    }
  }


template<class K,class D> inline
D* Btree<K,D>::remove(const K& oldKey,D& retData) {
  Btree_node<K,D>** pStack[STACK_SIZE];
  bool dStack[STACK_SIZE];
  short stackTop=-1;

  Btree_node<K,D>** cur=&root;
  Btree_node<K,D>* toDelete;

  while(*cur) {
    //cout<<"Current item in traversal: "<<(*cur)->data<<endl;
    if(oldKey<(*cur)->key) {
      pStack[++stackTop]=cur;
      dStack[stackTop]=0; //0=left
      cur=&(*cur)->c1;
      }
    else if(oldKey>(*cur)->key) {
      pStack[++stackTop]=cur;
      dStack[stackTop]=1; //1=right
      cur=&(*cur)->c2;
      }
    else break;
    }

  if(!(*cur)) throw "Element not found.";

  pStack[++stackTop]=cur;
  dStack[stackTop]=1;
  Btree_node<K,D>** temp=cur;

  cur=&(*cur)->c2;

  if(*cur) {
    do {
      pStack[++stackTop]=cur;
      dStack[stackTop]=0;
      cur=&(*cur)->c1;
      } while(*cur);
    cur=pStack[stackTop];
    retData=(*temp)->data;
    (*temp)->data=(*cur)->data;
    (*temp)->key=(*cur)->key;

    //cout<<"Deleting with inorder successor"<<endl;
    toDelete=*cur; //preserve *cur to be deleted

    (*cur)=(*cur)->c2; //excise cur
    toDelete->c2=0;
    delete toDelete;
    }
  else { //no rchild
    //cout<<"Deleting left"<<endl;
    toDelete=*temp;
    retData=(*temp)->data;
    (*temp)=(*temp)->c1;

    toDelete->c1=0;
    delete toDelete;
    }

  //cout<<"Rebalancing"<<endl<<flush;
  bool dir;
  stackTop--;

  while(stackTop>=0) {
    cur=pStack[stackTop];
    dir=dStack[stackTop--];

    //cout<<"At node "<<(*cur)->data<<" which went "<<(dir?"right":"left")<<endl<<flush;
    
    if(dir) { //right path
      if((--(*cur)->bal) == -2) {
        (*cur)->rbalance(*cur);
        if((*cur)->bal == 1) break;
        }
      else if((*cur)->bal == -1) break;
      }
    else { //left path
      if((++(*cur)->bal) == 2) {
        (*cur)->lbalance(*cur);
        if((*cur)->bal == -1) break;
        }
      else if((*cur)->bal == 1) break;
      }
    }
  return &retData;
  }



template<class K,class D> inline
void Btree<K,D>::remove(const K& oldKey) {
  //cout<<"Entering remove function"<<endl;
  Btree_node<K,D>** pStack[STACK_SIZE];
  bool dStack[STACK_SIZE];
  short stackTop=-1;

  Btree_node<K,D>** cur=&root;
  Btree_node<K,D>* toDelete;

  while(*cur) {
    //cout<<"Current item in traversal: "<<(*cur)->data<<endl;
    if(oldKey<(*cur)->key) {
      pStack[++stackTop]=cur;
      dStack[stackTop]=0; //0=left
      cur=&(*cur)->c1;
      }
    else if(oldKey>(*cur)->key) {
      pStack[++stackTop]=cur;
      dStack[stackTop]=1; //1=right
      cur=&(*cur)->c2;
      }
    else break;
    }

  if(!(*cur)) {
    if(onRemoveNotFound==EXCEPTION) throw "Element not found.";
    else return;
    }

  pStack[++stackTop]=cur;
  dStack[stackTop]=1;
  Btree_node<K,D>** temp=cur;

  cur=&(*cur)->c2;

  if(*cur) {
    do {
      pStack[++stackTop]=cur;
      dStack[stackTop]=0;
      cur=&(*cur)->c1;
      } while(*cur);
    cur=pStack[stackTop];
    (*temp)->data=(*cur)->data;
    (*temp)->key=(*cur)->key;

    //cout<<"Deleting with inorder successor"<<endl;
    toDelete=*cur; //preserve *cur to be deleted

    (*cur)=(*cur)->c2; //excise cur
    toDelete->c2=0;
    delete toDelete;
    }
  else { //no rchild
    //cout<<"Deleting left"<<endl;
    toDelete=*temp;
    (*temp)=(*temp)->c1;

    toDelete->c1=0;
    delete toDelete;
    }

  //cout<<"Rebalancing"<<endl<<flush;
  bool dir;
  stackTop--;

  while(stackTop>=0) {
    cur=pStack[stackTop];
    dir=dStack[stackTop--];

    //cout<<"At node "<<(*cur)->data<<" which went "<<(dir?"right":"left")<<endl<<flush;
    
    if(dir) { //right path
      if((--(*cur)->bal) == -2) {
        (*cur)->rbalance(*cur);
        if((*cur)->bal == 1) break;
        }
      else if((*cur)->bal == -1) break;
      }
    else { //left path
      if((++(*cur)->bal) == 2) {
        (*cur)->lbalance(*cur);
        if((*cur)->bal == -1) break;
        }
      else if((*cur)->bal == 1) break;
      }
    }
  }


//---Balance Left---
template<class K,class D> inline
void Btree_node<K,D>::lbalance(Btree_node<K,D>* &me) {
  if(c2->bal<0) {
    c2->rbalance(c2);
    }
  if(c2->bal==1) {
    if(bal==1) bal=c2->bal=-1;
    else c2->bal=bal=0;
    }
  else {
    c2->bal=-1;
    bal--;
    }

  Btree_node<K,D>* temp=c2;
  c2=c2->c1;
  temp->c1=me;
  me=temp;
  }


//---Balance Right---
template<class K,class D> inline
void Btree_node<K,D>::rbalance(Btree_node<K,D>* &me) {
  if(c1->bal>0) {
    c1->lbalance(c1);
    }
  if(c1->bal==-1) {
    if(bal==-1) bal=c1->bal=1;
    else c1->bal=bal=0;
    }
  else {
    c1->bal=1;
    bal++;
    }

  Btree_node<K,D>* temp=c1;
  c1=c1->c2;
  temp->c2=me;
  me=temp;
  }


template<class K,class D>
D* Btree<K,D>::get(const K &key,D& retData) {
  Btree_node<K,D>* cur=root;
  while(cur) {
    if     (key< cur->key) cur=cur->c1;
    else if(key> cur->key) cur=cur->c2;
    else {
      retData=cur->data;
      return &retData;
    }    
  }
  throw "Element not found.";
}


template<class K,class D>
bool Btree<K,D>::has(const K& key) const {
  Btree_node<K,D>* cur=root;
  while(cur) {
    if     (key< cur->key) cur=cur->c1;
    else if(key> cur->key) cur=cur->c2;
    else return true;    
  }
  return false;
}


template<class K,class D>
D& Btree<K,D>::get(const K& key) const {
  Btree_node<K,D>* cur=root;
  while(cur) {
    if     (key< cur->key) cur=cur->c1;
    else if(key> cur->key) cur=cur->c2;
    else return cur->data;
  }
  throw "Element not found.";
}


template<class K,class D>
D* Btree<K,D>::get_any(D& retData) const {
  if(root) {
    retData=root->data;
    return &retData;
  }
  throw "Element not found.";
}


template<class K,class D>
D* pop_any(D& retData) {
  if(!root) throw "Element not found.";
  return remove(root->data,retData);
}



//D* pop_min(D& retData);
//D* pop_max(D& retData);


template<class K,class D>
void Btree_node<K,D>::traverse(void f(const K& key,D& data)) {
  if(c1) c1->traverse(f);
  f(key,data);
  if(c2) c2->traverse(f);
  }

template<class K,class D>
void Btree_node<K,D>::traverse(void f(const K& key)) const {
  if(c1) c1->traverse(f);
  f(key);
  if(c2) c2->traverse(f);
  }

template<class K,class D>
void Btree_node<K,D>::traverse(void f(D& data)) {
  if(c1) c1->traverse(f);
  f(data);
  if(c2) c2->traverse(f);
  }


#endif
