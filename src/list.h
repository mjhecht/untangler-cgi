#ifndef LIST_H
#define LIST_H

#include "exception.h"
#include "f_assert.h"

class List_exception: public Exception {
public:
  List_exception(const string& value):Exception(value) {}
};

template<class T> class List;

template<class T>
class ListNode {
  friend class List<T>;
public:
  T item;
  ListNode(const T& item0,ListNode* next0=0):item(item0),next(next0) {}
private:
  ListNode* next;
};



template<class T>
class List {
private:
  ListNode<T>* head;
  int lastAccess;
  int mysize;
  ListNode<T>* lastAccessPointer;

public:
  List():head(NULL),mysize(0),lastAccess(-1) {}
  ~List() {
    ListNode<T>* cur=head,* prev;
    while(cur) {
      prev=cur;
      cur=cur->next;
      delete prev;
    }
  }
  List(const List& list0):head(NULL),mysize(0),lastAccess(-1) {
    ListNode<T>* cur=list0.head;
    while(cur) {
      insert(cur->item);
      cur=cur->next;
    }
  }

  List& operator=(const List& list0) {
    if(head) {
      delete head;
      head=0;
    }
    lastAccess=-1;
    mysize=0;
    ListNode<T>* cur=list0.head;
    while(cur) {
      insert(cur->item);
      cur=cur->next;
    }
    return *this;
  }


  void insert(const T& item) {
    assert(head = new ListNode<T>(item,head));
    mysize++;
    if(lastAccess>=0) lastAccess++;
  }


  void remove(int index) {
    if(index<0 || index>=mysize) {
      throw List_exception(string("Requested item")+index+" from "+mysize+"-item list");
    }

    ListNode<T>* cur=head;
    if(index==0) {
      head=head->next;
      delete cur;
    }
    else {
      for(int i=0;i<index-1;i++) cur=cur->next;
      ListNode<T>* temp=cur->next;
      cur->next=cur->next->next;
      delete temp;
    }
    mysize--;
    lastAccess=-1;
  }


  void clear() {
    if(head) {
      delete head;
      head=0;
    }
    lastAccess=-1;
    mysize=0;
  }

  inline int size() const {
    return mysize;
  }

  T& operator[](int pos) {
    if(pos<0 || pos>=mysize) {
      throw List_exception(string("Requested item")+index+" from "+mysize+"-item list");
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