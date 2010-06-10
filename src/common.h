#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <iostream>
using namespace std;

#include "btree.h"

typedef unsigned char byte;

template<class K,class D> class Index: public Btree<K,D> {};

class Textmap: public Index<string,string> {
public:
  static void print_pair(const string& k,string& d) {cout<<k<<"="<<d<<endl;}
};

#endif
