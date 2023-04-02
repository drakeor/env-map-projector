// 010-TestCase.cpp
// And write tests in the same file:
#include <catch2/catch_test_macros.hpp>
#include <cstdint>

uint32_t factorial( uint32_t number ) {
    return number <= 1 ? number : factorial(number-1) * number;
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( factorial( 1) == 1 );
    REQUIRE( factorial( 2) == 2 );
    REQUIRE( factorial( 3) == 6 );
    REQUIRE( factorial(10) == 3'628'800 );
}

// Compile & run:
// - g++ -std=c++14 -Wall -I$(CATCH_SINGLE_INCLUDE) -o 010-TestCase 010-TestCase.cpp && 010-TestCase --success
// - cl -EHsc -I%CATCH_SINGLE_INCLUDE% 010-TestCase.cpp && 010-TestCase --success

// Expected compact output (all assertions):
//
// prompt> 010-TestCase --reporter compact --success
// 010-TestCase.cpp:14: failed: Factorial(0) == 1 for: 0 == 1
// 010-TestCase.cpp:18: passed: Factorial(1) == 1 for: 1 == 1
// 010-TestCase.cpp:19: passed: Factorial(2) == 2 for: 2 == 2
// 010-TestCase.cpp:20: passed: Factorial(3) == 6 for: 6 == 6
// 010-TestCase.cpp:21: passed: Factorial(10) == 3628800 for: 3628800 (0x375f00) == 3628800 (0x375f00)
// Failed 1 test case, failed 1 assertion.