#include "../src/Utils/ImageReader.h"
#include "../src/Coordinates/CoordConversions.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

using namespace EnvProj;

const float pi = 3.14159265358979323846f;
const float epsilon = 1e-3;

TEST_CASE( "CoordConversions", "[coords-conversions]" ) {

    SECTION( "case [1,0,0]" ) {

        Eigen::Vector3<float> cartPoint(1, 0, 0);
        PointSphere<float> spherePoint = CoordConversions<float>::CartesianToSpherical(cartPoint);
        REQUIRE(spherePoint.azimuth < epsilon);
        REQUIRE(spherePoint.elevation < epsilon);

        // Special case where we can directly compare our input
        // because the unit sphere intersects this side of the unit sphere
        // at this point
        Eigen::Vector3<float> rebuiltPoint =  CoordConversions<float>::SphericalToCartesian(spherePoint);
        REQUIRE(fabs(rebuiltPoint.x() - 1.0f) < epsilon);
        REQUIRE(fabs(rebuiltPoint.y()) < epsilon);
        REQUIRE(fabs(rebuiltPoint.z()) < epsilon);

    }


    SECTION( "case [0,1,-1]" ) {

        Eigen::Vector3<float> cartPoint(0, 1, -1);
        PointSphere<float> spherePoint = CoordConversions<float>::CartesianToSpherical(cartPoint);
        REQUIRE(fabs(spherePoint.azimuth - 1.5708f) < epsilon);
        REQUIRE(fabs(spherePoint.elevation + 0.7854f) < epsilon);

        // intersection from origin to unit cube on unit sphere, hence
        // the coordinates are not 0,1,-1 since length is 1.
        Eigen::Vector3<float> rebuiltPoint =  CoordConversions<float>::SphericalToCartesian(spherePoint);
        REQUIRE(fabs(rebuiltPoint.x()) < epsilon);
        REQUIRE(fabs(rebuiltPoint.y() - 0.7071f) < epsilon);
        REQUIRE(fabs(rebuiltPoint.z() + 0.7071f) < epsilon);

    }

    SECTION( "case [-1,-0.25,0.85]" ) {

        Eigen::Vector3<float> cartPoint(-1.0f, -0.25f, 0.85f);
        PointSphere<float> spherePoint = CoordConversions<float>::CartesianToSpherical(cartPoint);
        REQUIRE(fabs(spherePoint.azimuth + 2.8966f) < epsilon);
        REQUIRE(fabs(spherePoint.elevation - 0.6896f) < epsilon);

        // intersection from origin to unit cube on unit sphere, hence
        // the coordinates are not 0,1,-1 since length is 1.
        Eigen::Vector3<float> rebuiltPoint =  CoordConversions<float>::SphericalToCartesian(spherePoint);
        REQUIRE(fabs(rebuiltPoint.x() + 0.7485) < epsilon);
        REQUIRE(fabs(rebuiltPoint.y() + 0.1871) < epsilon);
        REQUIRE(fabs(rebuiltPoint.z() - 0.6362f) < epsilon);

    }


}
