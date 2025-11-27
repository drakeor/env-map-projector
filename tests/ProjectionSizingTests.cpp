#include "../src/Utils/ProjectionSizing.h"

#include <catch2/catch_test_macros.hpp>
#include <cmath>

TEST_CASE("ProjectionSizing round-trips ERP and cubemap", "[projection-sizing]")
{
    auto sizes = ProjectionSizing::FromEquirectangular(8192, 4096);
    REQUIRE(sizes.cubemapFace == 2048);
    REQUIRE(sizes.hemisphereSize == 4096);

    auto cube = ProjectionSizing::FromCubemapFace(sizes.cubemapFace);
    REQUIRE(cube.equirectWidth == 8192);
    REQUIRE(cube.equirectHeight == 4096);
    REQUIRE(cube.hemisphereSize == 4096);
}

TEST_CASE("ProjectionSizing round-trips hemispherical and cubemap", "[projection-sizing]")
{
    auto hemi = ProjectionSizing::FromHemispherical(4096);
    REQUIRE(hemi.cubemapFace == 2048);
    REQUIRE(hemi.equirectWidth == 8192);
    REQUIRE(hemi.equirectHeight == 4096);

    auto cube = ProjectionSizing::FromCubemapFace(hemi.cubemapFace);
    REQUIRE(cube.hemisphereSize == 4096);
}

TEST_CASE("ProjectionSizing clamps invalid inputs", "[projection-sizing]")
{
    auto fromZero = ProjectionSizing::FromEquirectangular(0, 0);
    REQUIRE(fromZero.equirectWidth >= 16);
    REQUIRE(fromZero.equirectHeight >= 16);
    REQUIRE(fromZero.cubemapFace >= 16);
    REQUIRE(fromZero.hemisphereSize >= 16);

    auto cube = ProjectionSizing::FromCubemapFace(0);
    REQUIRE(cube.equirectWidth >= 16);
    REQUIRE(cube.equirectHeight >= 16);
    REQUIRE(cube.hemisphereSize >= 16);
}

TEST_CASE("ProjectionSizing horizon parity sizing", "[projection-sizing]")
{
    using ProjectionSizing::HemisphereSizingMode;
    constexpr double kPi = 3.14159265358979323846;
    const double kSqrtPi = std::sqrt(kPi);

    auto sizes = ProjectionSizing::FromEquirectangular(16384, 8192, HemisphereSizingMode::ArtifactReduction);
    int expectedHemi = static_cast<int>(std::lround(16384.0 / kSqrtPi));
    REQUIRE(sizes.hemisphereSize == expectedHemi);
    REQUIRE(sizes.hemisphereSize > 8192); // larger than identity S when deriving from ERP

    auto hemi = ProjectionSizing::FromHemispherical(4096, HemisphereSizingMode::ArtifactReduction);
    int expectedWidth = static_cast<int>(std::lround(kSqrtPi * 4096.0));
    int expectedHeight = static_cast<int>(std::lround(expectedWidth * 0.5));
    int expectedFace = static_cast<int>(std::lround(expectedWidth * 0.25));
    REQUIRE(hemi.equirectWidth == expectedWidth);
    REQUIRE(hemi.equirectHeight == expectedHeight);
    REQUIRE(hemi.cubemapFace == expectedFace);
    REQUIRE(hemi.cubemapFace < 4096 / 2);

    auto cube = ProjectionSizing::FromCubemapFace(expectedFace, HemisphereSizingMode::ArtifactReduction);
    REQUIRE(std::abs(cube.hemisphereSize - hemi.hemisphereSize) <= 1);
}
