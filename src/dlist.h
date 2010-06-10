#ifndef DLIST_H
#define DLIST_H

#include "f_assert.h"

struct Dlist_x {};

template<class T> class Dlist;

template<class T>
class Dlist_node {
  friend class Dlist<T>;
public:
  T item;
  Dlist_node(const T& item0,Dlist_node* next0=0):item(item0),next(next0) {}
private:
  Dlist_node* next;
};



template<class T>
class Dlist {
private:
  Dlist_node<T>* head;
  int lastAccess;
  int mysize;
  Dlist_node<T>* lastAccessPointer;

  void free() {
    Dlist_node<T>* cur=head,* prev;
    while(cur) {
      prev=cur;
      cur=cur->next;
      delete prev;
    }
  }


public:
  Dlist():head(NULL),mysize(0),lastAccess(-1) {}
  
  ~Dlist() {
    free();
  }

  Dlist(const Dlist& list0):head(NULL),mysize(0),lastAccess(-1) {
    Dlist_node<T>* cur=list0.head;
    while(cur) {
      insert(cur->item);
      cur=cur->next;
    }
    reverse();
  }

  Dlist& operator=(const Dlist& list0) {
    if(&list0==this) return *this;
    clear();
    Dlist_node<T>* cur=list0.head;
    while(cur) {
      insert(cur->item);
      cur=cur->next;
    }
    reverse();
    return *this;
  }

  inline void push(const T& item) {
    insert(item);
  }

  void reverse() {
    int i;
    Dlist<T> temp;    
    for(i=0;i<size();i++) temp.push((*this)[i]);
    free();
    head=temp.head;
    temp.head=0;
    lastAccess=-1;
  }


  void insert(const T& item) {
    f_assert(head = new Dlist_node<T>(item,head));
    mysize++;
    if(lastAccess>=0) lastAccess++;
  }


  T& pop(T& item) {
    item=(*this)[0];
    remove(0);
    return item;
  }



  void remove(int index) {
    if(index<0 || index>=mysize) {
      cerr<<"Requested item "<<index<<" from "<<mysize<<"-item list"<<endl;      
      throw string("Requested out of bounds list item");
    }


    Dlist_node<T>* cur=head;
    if(index==0) {
      head=head->next;
      delete cur;
    }
    else {
      for(int i=0;i<index-1;i++) cur=cur->next;
      Dlist_node<T>* temp=cur->next;
      cur->next=cur->next->next;
      delete temp;
    }
    mysize--;
    lastAccess=-1;
  }


  void clear() {
    free();
    head=0;
    lastAccess=-1;
    mysize=0;
  }
  
  
  
  inline int size() const {
    return mysize;
  }


  const T& top() const {
    if(!head) throw string("Requested top item from empty list.");
    return head->item;
  }

  T& top() {
    if(!head) throw string("Requested top item from empty list.");
    return head->item;
  }


  const T& operator[](int pos) const {    
    Dlist_node<T>* cur=head;
    if(pos>=mysize) throw string("Requested item at position greater than list size.");
    for(int i=0;i<pos;i++) cur=cur->next;
    return cur->item;
  }


  T& operator[](int pos) {
    if(pos<0 || pos>=mysize) {
      cerr<<"Requested item "<<pos<<" from "<<mysize<<"-item list"<<endl;      
      throw string("Requested out of bounds list item.");
    }
    if(pos>=lastAccess && lastAccess!=-1) {
      for(int i=0;i<(pos-lastAccess);i++) lastAccessPointer=lastAccessPointer->next;
      lastAccess=pos;
      return lastAccessPointer->item;
    }
    lastAccessPointer=head;
    for(int i=0;i<pos;i++) lastAccessPointer=lastAccessPointer->next;
    lastAccess=pos;
    return lastAccessPointer->item;
  }


};




#endif
