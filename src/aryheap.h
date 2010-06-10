/*
Simple array-based heap template
Last modified 20 June 1999
Michael Hecht
*/

#ifndef ARYHEAP_CPP
#define ARYHEAP_CPP

class Heap_x {};
class Heap_overflow_x: Heap_x {};

#include <iostream>
#include <cstdlib>
using namespace std;


template<class T>
class AryHeap {
  private:
    T** heap;
    int size;
    int last;
  public:
    AryHeap(int s):last(0),size(s+1) {
      heap=new T*[s+1];
      if(!heap) {cout<<"Out of memory"<<endl; exit(1);}
    }
    ~AryHeap();
    void insert(T* newitem);
    T* remove();
    T* peek();
    int used() {return last;}
    void printinfo();
    void clear() {last=0;}

};

template<class T> inline void mswap(T* &a, T* &b) {T* temp=a; a=b; b=temp;}

template<class T>
void AryHeap<T>::insert(T* newitem) {
  if(last>=size) throw Heap_overflow_x();
  heap[++last]=newitem;
  T* temp;
  for(int i=last;i>1;i/=2) {
    if(*heap[i]<*heap[i/2]) {
      temp=heap[i/2];
      heap[i/2]=heap[i];
      heap[i]=temp;
    }
  }
}

template<class T>
T* AryHeap<T>::remove() {
  if(last<1) return 0;

  T* temp=heap[1];
  heap[1]=heap[last--];

  for(int i=1;2*i<=last;) {
    if(*heap[2*i]<*heap[i]) {
      if(2*i+1<=last && *heap[2*i+1]<*heap[2*i]) {
        mswap(heap[2*i+1],heap[i]);
        i=2*i+1;
      }
      else {
        mswap(heap[2*i],heap[i]);
        i=2*i;
      }
    }
    else if(2*i+1<=last && *heap[2*i+1]<*heap[i]) {
      mswap(heap[2*i+1],heap[i]);
      i=2*i+1;
    }
    else break;
  }
  return temp;
}

template<class T>
T* AryHeap<T>::peek() {
  if(last<1) return 0;
  return heap[1];
}

template<class T>
AryHeap<T>::~AryHeap() {
  for(int i=1;i<=last;i++) delete heap[i];
  delete[] heap;
}

template<class T>
void AryHeap<T>::printinfo() {
//  for(int i=1;i<=last;i++) cout<<(*heap[i])<<" ";
  }

#endif
