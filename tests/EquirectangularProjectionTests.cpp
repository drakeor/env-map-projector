#include "../src/EquirectangularProjection.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

TEST_CASE( "EquirectangularProjection", "[equirectangular-projection]" ) {

    // should not cause an error
    SECTION( "initialization" ) {
        EquirectangularProjection proj;
    }

}