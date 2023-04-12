#ifndef _COORDS_3D_CONTAINER_H_
#define _COORDS_3D_CONTAINER_H_

#include "../lib/nanoflann/include/nanoflann.hpp"
#include "Point3d.h"
#include "Point3dCloud.h"

//#define DEBUG_KD_TREE_OUTPUT

//  KD-Tree index
// TODO: Instead of L2_Simple_Adaptor, we would want to eventually choose
// something for spherical coordinates too.
using my_kd_tree_t = nanoflann::KDTreeSingleIndexDynamicAdaptor<
        nanoflann::L2_Simple_Adaptor<float, Point3dCloud<float>>, 
        Point3dCloud<float>, 
        3 /* dim */
        >;

class Coords3dContainer
{
public:
    Coords3dContainer(int containerSize);
    ~Coords3dContainer();

    // Slowest way to add. Should add in chunks versus resizing the vector
    // all the time
    int GetSize();
    bool SetPoint(int index, Point3d<float> point);
    bool IndexAllPoints();

    Point3d<float> GetClosestPoint(float x, float y, float z);

    std::vector<Point3d<float>> GetAllPoints();

private:
    Point3dCloud<float> cloud;
    my_kd_tree_t index;
    int allocatedSize;
};

#endif