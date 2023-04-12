#include "Coords3dContainer.h"

#include <iostream>

Coords3dContainer::Coords3dContainer(int containerSize)
    : index(3, cloud, { 10 })
{
    allocatedSize = containerSize;
    cloud.pts.reserve(containerSize);
}

Coords3dContainer::~Coords3dContainer()
{

}

int Coords3dContainer::GetSize()
{
    return allocatedSize;
}

bool Coords3dContainer::SetPoint(int index, Point3d<float> point)
{
    // Bounds checking
    if(index < 0 || index >= cloud.pts.size())
        return false;

    cloud.pts[index] = point;
    return true;
}

bool Coords3dContainer::IndexAllPoints()
{
    // Add all points to the KD-tree
    index.addPoints(0, cloud.pts.size()-1);
    return true;
}


std::vector<Point3d<float>> Coords3dContainer::GetAllPoints()
{
    return cloud.pts;
}


Point3d<float> Coords3dContainer::GetClosestPoint(float x, float y, float z)
{

    float query_pt[3] = {x, y, z};
    const size_t num_results = 1;
    size_t ret_index = 0;
    float out_dist_sqr = 0;
    nanoflann::KNNResultSet<float> resultSet(num_results);
    resultSet.init(&ret_index, &out_dist_sqr);
    index.findNeighbors(resultSet, query_pt, {10});

#ifdef DEBUG_KD_TREE_OUTPUT
    std::cout << "input point (" << x << "," << y << "," << z << ")" << std::endl;
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
