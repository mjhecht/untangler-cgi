#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

struct Exception {
  std::string message;
  Exception() {}
  Exception(string s):message(s) {}
};

struct File_exception: Exception {};
struct Math_exception: Exception {};

struct File_notfound_exception: File_exception {};
struct File_create_exception: File_exception {};

/*
class Exception {
protected:
  string _value;
public:
  string value() const { return _value; }
  void value(const string& value) { _value = value; }

  Exception(const string& value):_value(value) {}
};

class File_exception: public Exception {
public:
  File_exception() {}
  File_exception(const string& value):Exception(value) {}
};

class Math_exception: public Exception {
public:
  Math_exception() {}
  Math_exception(const string& value):Exception(value) {}
};
*/

#endif
