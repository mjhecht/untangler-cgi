/* simple list-based stack "/misc/stack.h" */
/* includes implementation */

#ifndef stack_h
#define stack_h

#include <cstdlib>
#include <iostream>
using namespace std;

#include "exception.h"

struct Stack_exception: Exception {};
struct Stack_empty_exception: Stack_exception {};


template<class T>
class Stack;

template<class T>
class StackItem {
  friend class Stack<T>;
  StackItem<T>* next;
  T data;

  public:
  StackItem(T d,StackItem<T>* n): data(d),next(n) {}
  };

template<class T>
class Stack {
  StackItem<T>* head;

  public:
  Stack(): head(0) {}
  void push(T data) {head=new StackItem<T>(data,head); }
  T pop();
  T top();
  bool inuse() {return head?1:0;}
  };


template<class T>
T Stack<T>::pop() {
  if(!head) throw Stack_empty_exception();
  T temp=head->data;
  StackItem<T>* first=head;
  head=head->next;
  delete first;
  return temp;
  }

template<class T>
T Stack<T>::top() {
  if(!head) throw Stack_empty_exception();
  return head->data;
  }




#endif
