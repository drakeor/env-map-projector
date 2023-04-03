#include "CoordsContainer2d.h"

CoordsContainer2d::CoordsContainer2d()
    : index(2, cloud, { 10 })
{

}

CoordsContainer2d::~CoordsContainer2d()
{

}

void CoordsContainer2d::AddPoint(float x, float y, unsigned int value)
{
    cloud.pts.push_back({
        x, y, value
    });
}