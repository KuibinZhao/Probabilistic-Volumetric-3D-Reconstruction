// This is ./vxl/vnl/vnl_sym_matrix.txx
#ifndef vnl_sym_matrix_txx_
#define vnl_sym_matrix_txx_

//:
// \file

#include "vnl_sym_matrix.h"
#include <vcl_iostream.h>


// ==========================================================================
//: Replaces the symetric submatrix of THIS matrix, starting at top left corner, by the elements of matrix m.
// O(m*m).
template<class T>
vnl_sym_matrix<T>& vnl_sym_matrix<T>::update (vnl_sym_matrix<T> const& m,
  unsigned diagonal_start)
{
  unsigned int end = diagonal_start + m.nn_;
#if VNL_CONFIG_CHECK_BOUNDS  && (!defined NDEBUG)
  if (this->nn_ < end)
    vnl_error_matrix_dimension ("vnl_sym_matrix::update",
                                end, end, m.nn_, m.nn_);
#endif
  for (unsigned int i = diagonal_start; i < end; i++)
    for (unsigned int j = diagonal_start; j <= i; j++)
      this->fast(i,j) = m.fast(i-diagonal_start,j-diagonal_start);
  return *this;
}

// ==========================================================================
//: Swap contents of m with THIS
template <class T>
void vnl_sym_matrix<T>::swap(vnl_sym_matrix<T> &m)
{
  unsigned nn = nn_;
  T **index   = index_;
  T *data     = data_;
  nn_    =m.nn_;
  index_ =m.index_;
  data_  =m.data_;
  m.nn_    =nn;
  m.index_ =index;
  m.data_  =data;
}

// ==========================================================================

template <class T>
vnl_sym_matrix<T>& vnl_sym_matrix<T>::operator=(vnl_sym_matrix<T> const& that)
{
  if (&that == this) return *this;

  resize(that.rows());
  update(that);
  return *this;
}

// ==========================================================================
//: Set the first i values of row i
// or the top i values of column i
template <class T>
void vnl_sym_matrix<T>::set_half_row (const vnl_vector<T> &half_row, unsigned i)
{
#if VNL_CONFIG_CHECK_BOUNDS  && (!defined NDEBUG)
  if (half_row.size() != i+1)
    vnl_error_vector_dimension ("vnl_sym_matrix::set_half_row wrong size for half row",
                                half_row.size(), i+1);
  if ( i > nn_)
    vnl_error_vector_dimension ("vnl_sym_matrix::set_half_row wrong sizes",
                                i+1, rows());
#endif
  half_row.copy_out(index_[i]);
}

// ==========================================================================
//: print in lower triangular form
template <class T>
vcl_ostream& operator<< (vcl_ostream& s, const vnl_sym_matrix<T>& M)
{
  for (unsigned i=0; i<M.rows(); ++i)
  {
    for (unsigned j=0; j<=i; ++j)
      s << M.fast(i,j) << " ";
    s  << vcl_endl;
  }
  return s;
}

// ==========================================================================

template <class T>
bool operator==(const vnl_sym_matrix<T> &a, const vnl_sym_matrix<T> &b)
{
  if (a.rows() != b.rows()) return false;
  const T* a_data = a.data_block();
  const T* b_data = a.data_block();
  const unsigned mn = a.size();
  for (unsigned i = 0; i < mn; ++i)
    if (a_data[i] != b_data[i]) return false;
  return true;
}

// ==========================================================================

template <class T>
bool operator==(const vnl_sym_matrix<T> &a, const vnl_matrix<T> &b)
{
  if (a.rows() != b.rows() || a.cols() != b.cols()) return false;
  //const T* a_data = a.data_block();
  //const T* b_data = a.data_block();
  const unsigned n = a.rows();
  for (unsigned i=0; i< n; ++i)
    for (unsigned j=0; j<=i; ++j)
      if (a.fast(i,j) != b(i,j) || a.fast(i,j) != b(j,i)) return false;
  return true;
}

// ==========================================================================

template <class T>
bool operator==(const vnl_matrix<T> &a, const vnl_sym_matrix<T> &b)
{
  return operator==(b,a);
}

// ==========================================================================

#undef VNL_SYM_MATRIX_INSTANTIATE
#define VNL_SYM_MATRIX_INSTANTIATE(T) \
template class vnl_sym_matrix<T >; \
template vcl_ostream& operator<< (vcl_ostream& s, vnl_sym_matrix<T > const &); \
template bool operator==(const vnl_sym_matrix<T > &a, const vnl_sym_matrix<T > &b); \
template bool operator==(const vnl_sym_matrix<T > &a, const vnl_matrix<T > &b); \
template bool operator==(const vnl_matrix<T > &a, const vnl_sym_matrix<T > &b)
#endif
