#include "searchwrapper.h"
#include <pcl/features/boundary.h>

#define BoundaryEstimation_ptr pcl::BoundaryEstimation<_PointT, pcl::Normal, pcl::Boundary>::Ptr
#define PointCloud_ptr pcl::PointCloud<_PointT>::Ptr
#define Normals_ptr pcl::PointCloud<pcl::Normal>::Ptr

#define KdTree_ptr pcl::KdTreeFLANN<_PointT>::Ptr
#define Octree_ptr pcl::octree::OctreePointCloudSearch<_PointT>::Ptr

PCLIMP(void*, BoundaryEstimation, new)()
{
  return new BoundaryEstimation_ptr(new pcl::BoundaryEstimation<_PointT, pcl::Normal, pcl::Boundary>());
}

PCLIMP(void, BoundaryEstimation, delete)(BoundaryEstimation_ptr *self)
{
  delete self;
}

PCLIMP(void, BoundaryEstimation, setInputCloud)(BoundaryEstimation_ptr *self, PointCloud_ptr *cloud)
{
  (*self)->setInputCloud(*cloud);
}

PCLIMP(void, BoundaryEstimation, setInputNormals)(BoundaryEstimation_ptr *self, Normals_ptr *normals)
{
  (*self)->setInputNormals(*normals);
}

PCLIMP(void, BoundaryEstimation, setIndices)(BoundaryEstimation_ptr *self, Indices_ptr *indices)
{
  (*self)->setIndices(*indices);
}

PCLIMP(void, BoundaryEstimation, setAngleThreshold)(BoundaryEstimation_ptr *self, float angle)
{
  (*self)->setAngleThreshold(angle);
}

PCLIMP(float, BoundaryEstimation, getAngleThreshold)(BoundaryEstimation_ptr *self)
{
  return (*self)->getAngleThreshold();
}

PCLIMP(void, BoundaryEstimation, setSearchMethod_Octree)(BoundaryEstimation_ptr *self, Octree_ptr *octree)
{
  boost::shared_ptr<OctreeSearchWrapper<_PointT> > wrapper(new OctreeSearchWrapper<_PointT>(*octree));
  (*self)->setSearchMethod(wrapper);
}

PCLIMP(void, BoundaryEstimation, setSearchMethod_KdTree)(BoundaryEstimation_ptr *self, KdTree_ptr *kdtree)
{
  boost::shared_ptr<KdTreeSearchWrapper<_PointT> > wrapper(new KdTreeSearchWrapper<_PointT>(*kdtree));
  (*self)->setSearchMethod(wrapper);
}

PCLIMP(void, BoundaryEstimation, setKSearch)(BoundaryEstimation_ptr *self, int k)
{
  (*self)->setKSearch(k);
}

PCLIMP(int, BoundaryEstimation, getKSearch)(BoundaryEstimation_ptr *self)
{
  return (*self)->getKSearch();
}

PCLIMP(void, BoundaryEstimation, setRadiusSearch)(BoundaryEstimation_ptr *self, double radius)
{
  (*self)->setRadiusSearch(radius);
}

PCLIMP(double, BoundaryEstimation, getRadiusSearch)(BoundaryEstimation_ptr *self)
{
  return (*self)->getRadiusSearch();
}

PCLIMP(void, BoundaryEstimation, compute)(BoundaryEstimation_ptr *self, THByteTensor *output)
{
  pcl::PointCloud<pcl::Boundary> output_;
  (*self)->compute(output_);
  
  // todo: copy boundary points to output tensor
  
}

#undef BoundaryEstimation_ptr
#undef PointCloud_ptr
#undef Normals_ptr
#undef KdTree_ptr
#undef Octree_ptr
