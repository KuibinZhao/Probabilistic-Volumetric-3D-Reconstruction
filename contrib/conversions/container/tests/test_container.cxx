#include <container/vcl_list_to_vcl_vector.h>
#include <container/vcl_vector_to_vcl_list.h>
#include <container/vcl_vector_to_vnl_vector.h>
#include <container/vnl_vector_to_vcl_vector.h>
#include <vnl/vnl_test.h>

void test_container_conversions()
{
  int data[] = { 1, 2, 3, 2, 1, 0, -1 };
  vcl_vector<int> l(data, data+7);
  vcl_list<int>   a = vcl_vector_to_vcl_list(l);
  TEST("container conversions: begin()", *(a.begin()), 1);
  TEST("container conversions: size()", a.size(), 7);
  vcl_vector<int> b = vcl_list_to_vcl_vector(a);
  TEST("container conversions: vcl_vector", b[0]==1 && b[1]==2 && b[2]==3 && b[3]==2 && b[4]==1 && b[5]==0 && b[6]==-1, true);
  vnl_vector<int> c = vcl_vector_to_vnl_vector(b);
  TEST("container conversions: vnl_vector", c[0]==1 && c[1]==2 && c[2]==3 && c[3]==2 && c[4]==1 && c[5]==0 && c[6]==-1, true);
  vcl_vector<int> d = vnl_vector_to_vcl_vector(c);
  TEST("container conversions: vcl_vector", b, d);
}

TESTMAIN(test_container_conversions);
