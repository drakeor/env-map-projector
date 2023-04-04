#include "CoordsContainer2d.h"

#include <iostream>

CoordsContainer2d::CoordsContainer2d()
    : index(2, cloud, { 10 })
{

}

CoordsContainer2d::~CoordsContainer2d()
{

}

void CoordsContainer2d::AddPoint(float x, float y, unsigned int value)
{
    // This is probably inefficient
    cloud.pts.push_back({
        x, y, value
    });
    index.addPoints(cloud.pts.size()-1, cloud.pts.size()-1);
}

Point2d<float> CoordsContainer2d::GetClosestPoint(float x, float y)
{

    float query_pt[2] = {x, y};
    const size_t num_results = 1;
    size_t ret_index = 0;
    float out_dist_sqr = 0;
    nanoflann::KNNResultSet<float> resultSet(num_results);
    resultSet.init(&ret_index, &out_dist_sqr);
    index.findNeighbors(resultSet, query_pt, {10});

    std::cout << "knnSearch(nn=" << num_results << "): \n";
    std::cout << "ret_index=" << ret_index
                << " out_dist_sqr=" << out_dist_sqr << std::endl;
    std::cout << "point: ("
                << "point: (" << cloud.pts[ret_index].x << ", "
                << cloud.pts[ret_index].y
                << ")" << std::endl;
    std::cout << std::endl;

    return cloud.pts[ret_index];
}
