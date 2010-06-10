#ifndef MATRIX_H
#define MATRIX_H

#include "exception.h"
#include "assert.h"

struct Matrix_exception: public Exception {};
struct Matrix_dimension_exception: public Matrix_exception {};
struct Matrix_bounds_exception: public Matrix_exception {};


template<class T>
class Matrix {
  protected:
    T** data;
    int _rows;
    int _cols;
  public:
    int rows() const { return _rows; }
    int cols() const { return _cols; }

    Matrix():_rows(0),_cols(0) {}
    Matrix(int rows,int cols);
    ~Matrix();
    void resize(int rows,int cols);

    Matrix& operator=(const Matrix& m);


    //bracket operator, allows use of matrix[y][x] instead of matrix.data[y][x]
    T* operator[](int i) {return data[i];}
    inline bool is_in(int row,int col);
    void clear();
    void clear(T fill);
  };


template<class T>
inline bool Matrix<T>::is_in(int row,int col) {
  return (row>=0 && col>=0 && row<_rows && col<_cols);
  }


//constructor
template<class T>
Matrix<T>::Matrix(int rows,int cols):_rows(rows),_cols(cols) {
  if(_rows<1 || _cols<1) {
    throw "Matrix out of bounds exception.";
  }
  int i;
  data=new T*[rows];
  for(i=0;i<rows;i++) data[i]=new T[cols];
  }


template<class T>
Matrix<T>::~Matrix() {
  int i;
  for(i=0;i<_rows;i++) delete[] data[i];
}


//resize
template<class T>
void Matrix<T>::resize(int rows,int cols) {
  if(rows<=0 || cols<=0) {
    throw "Matrix out of bounds exception.";
  }

  /*
  if(_cols==cols) {
    if(_rows==rows) return;
    T** new_rows = new T*[rows];
    for(i=0;i<_rows;i++) new_rows[i]=data[i];
    for(;i<rows;i++) new_rows[i]=new T[cols];
    T** temp=data;
    data=new_rows;
    delete[] temp;
  }
  else {
    if(_rows==rows) {
      for(i=0;i<_rows;i++) {
        delete[] data[i];
        data[i]=new T[cols];
      }
      */
  int i;
  if(_rows>0) {
    for(i=0;i<_rows;i++) delete[] data[i];
    delete[] data;
  }
  assert(data = new T*[rows]);
  for(i=0;i<rows;i++) assert(data[i]=new T[cols]);
  _rows=rows;
  _cols=cols;
}


//clear matrix
template<class T>
void Matrix<T>::clear() {
  int i,j;
  for(i=0;i<_rows;i++) for(j=0;j<_cols;j++) data[i][j]=0;
  }

template<class T>
void Matrix<T>::clear(T fill) {
  int i,j;
  for(i=0;i<_rows;i++) for(j=0;j<_cols;j++) data[i][j]=fill;
  }


#endif
