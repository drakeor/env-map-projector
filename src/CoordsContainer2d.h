#ifndef _COORDS_CONTAINER2D_H_
#define _COORDS_CONTAINER2D_H_

#include "Point2d.h"
#include "PointCloud2d.h"
#include "../lib/nanoflann/include/nanoflann.hpp"

//#define DEBUG_KD_TREE_OUTPUT

//  KD-Tree index
// TODO: Instead of L2_Simple_Adaptor, we would want to eventually choose
// something for spherical coordinates too.
using my_kd_tree_t = nanoflann::KDTreeSingleIndexDynamicAdaptor<
        nanoflann::L2_Simple_Adaptor<float, PointCloud2d<float>>, 
        PointCloud2d<float>, 
        2 /* dim */
        >;

class CoordsContainer2d
{
public:
    CoordsContainer2d();
    ~CoordsContainer2d();

    // Slowest way to add. Should add in chunks versus resizing the vector
    // all the time
    void AddPoint(float x, float y, unsigned int value);
    void Empty();
    Point2df GetClosestPoint(float x, float y);

    std::vector<Point2d<float>> GetAllPoints();

private:
    PointCloud2d<float> cloud;
    my_kd_tree_t index;
};

#endif