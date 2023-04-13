#include "SphereCoordsContainer.h"

#include <iostream>

SphereCoordsContainer::SphereCoordsContainer(int requestedSize)
    : index(2, cloud, { 50 })
{
    cloud.pts.reserve(requestedSize);
    allocatedSize = requestedSize;
}

SphereCoordsContainer::~SphereCoordsContainer()
{

}

bool SphereCoordsContainer::AddPoint(SpherePoint<float> point)
{
    /*if(index < 0 || index >= allocatedSize)
        return false;

    cloud.pts[index] = point;*/
    cloud.pts.push_back(point);
    return true;
}

std::vector<SpherePoint<float>> SphereCoordsContainer::GetAllPoints()
{
    return cloud.pts;
}

bool SphereCoordsContainer::IndexAllPoints()
{
    index.addPoints(0, cloud.pts.size());    
    return true;
}

SpherePoint<float> SphereCoordsContainer::GetClosestPoint(float azim, float elev)
{

    float query_pt[2] = {azim, elev};
    const size_t num_results = 1;
    size_t ret_index = 0;
    float out_dist_sqr = 0;
    nanoflann::KNNResultSet<float> resultSet(num_results);
    resultSet.init(&ret_index, &out_dist_sqr);
    index.findNeighbors(resultSet, query_pt, {10});

#ifdef DEBUG_KD_TREE_OUTPUT
    std::cout << "input point (" << azim << "," << elev << ")" << std::endl;
    std::cout << "knnSearch(nn=" << num_results << "): \n";
    std::cout << "ret_index=" << ret_index
                << " out_dist_sqr=" << out_dist_sqr << std::endl;
    std::cout << "point: ("
                << "point: (" << cloud.pts[ret_index].azim << ", "
                << cloud.pts[ret_index].elev
                << ")" << std::endl;
    std::cout << std::endl;
#endif

    return cloud.pts[ret_index];
}
