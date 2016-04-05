#ifndef std::list_to_CoolArray_h_
#define std::list_to_CoolArray_h_

#include <iostream>
#include <list>
#include <cool/ArrayP.h>
#include <vcl_compiler.h>

template <class T>
inline
CoolArray<T> std::list_to_CoolArray(std::list<T> const& cl)
{
  CoolArray<T> l;
  typename std::list<T>::const_iterator it = cl.begin();
  for (; !(it == cl.end()); ++it)
    l.push(*it);
  return l;
}

#endif // std::list_to_CoolArray_h_
