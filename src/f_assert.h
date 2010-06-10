#ifndef F_ASSERT_H
#define F_ASSERT_H

#include <iostream>
#include <cstdlib>
using namespace std;

template<class T>
void f_assert(T ptr) {
  if(!ptr) {
    cout<<"Assertion failed (out of memory?)"<<endl;
    exit(1);
  }
}


#endif
