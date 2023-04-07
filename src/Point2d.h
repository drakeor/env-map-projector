#ifndef _POINT2D_H_
#define _POINT2D_H_

template <typename T>
struct Point2d
{
    T x, y;
    unsigned int pixelValue;
};

using Point2df = Point2d<float>;

#endif