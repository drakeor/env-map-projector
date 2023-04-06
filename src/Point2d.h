#ifndef _POINT2D_H_
#define _POINT2D_H_

template <typename T>
struct Point2d
{
    T x, y;
    unsigned int pixelValue;
};

typedef Point2d<float> Point2df;

#endif