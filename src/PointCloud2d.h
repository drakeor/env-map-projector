#ifndef _POINTCLOUD_H_
#define _POINTCLOUD_H_

#include <cstdlib>
#include <iostream>
#include <vector>

template <typename T>
struct PointCloud2d
{
    struct Point2d
    {
        T x, y;
        unsigned int pixelValue;
    };

    using coord_t = T;  //!< The type of each coordinate

    std::vector<Point2d> pts;

    // Must return the number of data points
    inline size_t kdtree_get_point_count() const { return pts.size(); }

    // Returns the dim'th component of the idx'th point in the class:
    // Since this is inlined and the "dim" argument is typically an immediate
    // value, the
    //  "if/else's" are actually solved at compile time.
    inline T kdtree_get_pt(const size_t idx, const size_t dim) const
    {
        if (dim == 0)
            return pts[idx].x;
        else
            return pts[idx].y;
    }

    // Optional bounding-box computation: return false to default to a standard
    // bbox computation loop.
    //   Return true if the BBOX was already computed by the class and returned
    //   in "bb" so it can be avoided to redo it again. Look at bb.size() to
    //   find out the expected dimensionality (e.g. 2 or 3 for point clouds)
    template <class BBOX>
    bool kdtree_get_bbox(BBOX& /* bb */) const
    {
        return false;
    }
};

template <typename T>
void generateRandomPointCloud(
    PointCloud2d<T>& point, const size_t N, const T max_range = 10)
{
    // Generating Random Point Cloud
    point.pts.resize(N);
    for (size_t i = 0; i < N; i++)
    {
        point.pts[i].x = max_range * (rand() % 1000) / T(1000);
        point.pts[i].y = max_range * (rand() % 1000) / T(1000);
    }
}

#endif