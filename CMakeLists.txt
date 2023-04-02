cmake_minimum_required (VERSION 3.0.2)

project (EnvMapProjector)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

add_subdirectory(lib/Catch2)

add_executable (envmapproj src/main.cpp)

add_executable (envmapproj_tests tests/test_main.cpp)
target_link_libraries(envmapproj_tests PRIVATE Catch2::Catch2WithMain)
file(COPY assets DESTINATION bin)