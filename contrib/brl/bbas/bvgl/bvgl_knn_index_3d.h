#ifndef bvgl_knn_index_3d_h_
#define bvgl_knn_index_3d_h_
//:
// \file
// \brief  A simple grid index to store a pointset, optionally with normals and optional scalar value
//
// \author J.L. Mundy
// \date   24 February 2016
//
#include <vcl_iostream.h>
#include <vcl_fstream.h>
#include <vcl_cassert.h>
#include <vcl_vector.h>
#include <vcl_string.h>
#include <vcl_map.h>
#include <vgl/vgl_point_3d.h>
#include <vgl/vgl_vector_3d.h>
#include <vgl/vgl_pointset_3d.h>
#include <vgl/vgl_box_3d.h>
#include <vgl/vgl_closest_point.h>
#include "bvgl_k_nearest_neighbors_3d.h"
template <class Type>
class bvgl_knn_index_3d{
 public:
 bvgl_knn_index_3d():has_scalars_(false),thresh_(Type(1)){}
  bvgl_knn_index_3d(vgl_pointset_3d<Type> ptset, Type thresh = Type(1));
  bvgl_knn_index_3d(vgl_pointset_3d<Type> ptset, vcl_vector<Type> scalars, Type thresh = Type(1));

  //: geometric queries
  //: the point contained in the grid closest to p or optionally to the normal plane passing through the closest point
  bool closest_point(vgl_point_3d<Type> const& p, vgl_point_3d<Type>& pc) const;
  bool closest_point(vgl_point_3d<Type> const& p, vgl_point_3d<Type>& pc, Type& scalar) const;
  //: the distance from p to the closest point or optionally its normal plane
  Type distance(vgl_point_3d<Type> const& p) const;
  Type distance(vgl_point_3d<Type> const& p, Type& scalar) const;
  //: accessors
  vgl_box_3d<Type> bounding_box() const {return bbox_;}
  unsigned npts() const {return knn_.const_ptset().npts();}
  bool has_normals() const {return knn_.const_ptset().has_normals_;}
  bool has_scalars() const {return has_scalars_;}
  //: accessors, allows efficient access to a single copy of the pointset stored in knn_
  const vgl_pointset_3d<Type>& const_ptset()const {return knn_.const_ptset();}
  vgl_pointset_3d<Type>& ptset(){return knn_.ptset();}
  void set_pointset(vgl_pointset_3d<Type> const& ptset){knn_.set_pointset(ptset);}
  void set_thresh(Type thresh){thresh_ = thresh;}
  void set_scalars(vcl_vector<Type> const& scalars){scalars_ = scalars; has_scalars_ = true;}
  bool create(){return knn_.create();}
 protected:
  bool has_scalars_;
  vgl_box_3d<Type> bbox_;
  vcl_vector<Type> scalars_;
  bvgl_k_nearest_neighbors_3d<Type> knn_;
  Type thresh_;
};
// implementation relatively short so include in the .h file
//: constructors
template <class Type>
bvgl_knn_index_3d<Type>::bvgl_knn_index_3d(vgl_pointset_3d<Type> ptset, Type thresh):
has_scalars_(false), thresh_(thresh){
  bbox_ = vgl_bounding_box(ptset);
  knn_.set_pointset(ptset);
}

template <class Type>
bvgl_knn_index_3d<Type>::bvgl_knn_index_3d(vgl_pointset_3d<Type> ptset, vcl_vector<Type> scalars, Type thresh):
has_scalars_(true), thresh_(thresh), scalars_(scalars){
  bbox_ = vgl_bounding_box(ptset);
  knn_.set_pointset(ptset);
}
//: the point contained in the grid closest to p or optionally to the normal plane passing through the closest point
template <class Type>
bool bvgl_knn_index_3d<Type>::closest_point(vgl_point_3d<Type> const& p, vgl_point_3d<Type>& pc, Type& scalar) const{
  scalar = 0.0;
  const vgl_pointset_3d<Type>& pst = knn_.const_ptset();
  if(!pst.npts())
    return false;
  unsigned index = 0;
  knn_.closest_index(p, index);
  vgl_point_3d<Type> pmin = pst.p(index);
  if(!pst.has_normals()){
    pc = pmin;
    if(this->has_scalars())
      scalar = scalars_[index];
    return true;
  }
  vgl_vector_3d<double> nc = pst.n(index);
  vgl_plane_3d<Type> pl(nc, pmin);
  pc = vgl_closest_point(pl, p);
  if((pc-pmin).length()>thresh_)
    pc = pmin;
  if(this->has_scalars())
    scalar = scalars_[index];
  return true;
}
template <class Type>
bool bvgl_knn_index_3d<Type>::closest_point(vgl_point_3d<Type> const& p, vgl_point_3d<Type>& pc) const{
  Type scalar = Type(0);
  return this->closest_point(p, pc, scalar);
}
template <class Type>
Type bvgl_knn_index_3d<Type>::distance(vgl_point_3d<Type> const& p, Type& scalar) const{
  vgl_point_3d<Type> pc;
  bool good = this->closest_point(p, pc, scalar);
  if(!good)
    return vcl_numeric_limits<Type>::max();
  return (p-pc).length();
}
//: the distance from p to the closest point or optionally its normal plane
template <class Type>
Type bvgl_knn_index_3d<Type>::distance(vgl_point_3d<Type> const& p) const{
  Type scalar = Type(0);
  return this->distance(p, scalar);
}

#endif// bvgl_knn_index_3d
