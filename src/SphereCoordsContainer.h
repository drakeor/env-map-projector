#ifndef _SPHERE_COORDS_CONTAINER_H_
#define _SPHERE_COORDS_CONTAINER_H_

#include "SpherePoint.h"
#include "SpherePointCloud.h"
#include "../lib/nanoflann/include/nanoflann.hpp"

//#define DEBUG_KD_TREE_OUTPUT

//  KD-Tree index
// TODO: Instead of L2_Simple_Adaptor, we would want to eventually choose
// something for spherical coordinates too.
using my_kd_tree_t = nanoflann::KDTreeSingleIndexDynamicAdaptor<
        nanoflann::L2_Simple_Adaptor<float, SpherePointCloud<float>>, 
        SpherePointCloud<float>, 
        2 /* dim */
        >;

class SphereCoordsContainer
{
public:
    SphereCoordsContainer();
    ~SphereCoordsContainer();

    // Slowest way to add. Should add in chunks versus resizing the vector
    // all the time
    void AddPoint(float azim, float elev, unsigned int value);
    void Empty();
    SpherePoint<float> GetClosestPoint(float azim, float elev);

    std::vector<SpherePoint<float>> GetAllPoints();

private:
    SpherePointCloud<float> cloud;
    my_kd_tree_t index;
};

#endif