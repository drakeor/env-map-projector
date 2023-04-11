#include "SphereCoordsContainer.h"

#include <iostream>

SphereCoordsContainer::SphereCoordsContainer()
    : index(2, cloud, { 10 })
{

}

SphereCoordsContainer::~SphereCoordsContainer()
{

}

void SphereCoordsContainer::AddPoint(float azim, float elev, unsigned int value)
{
    // This is probably inefficient
    cloud.pts.push_back({
        azim, elev, value
    });
    index.addPoints(cloud.pts.size()-1, cloud.pts.size()-1);
}

void SphereCoordsContainer::Empty()
{
    for(int i = 0; i < cloud.pts.size(); i++)
    {
        index.removePoint(i);
    }
    cloud.pts.clear();
}

std::vector<SpherePoint<float>> SphereCoordsContainer::GetAllPoints()
{
    return cloud.pts;
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
