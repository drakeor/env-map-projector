cmake_minimum_required (VERSION 3.0.2)

project (EnvMapProjector)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-O3 ${CMAKE_CXX_FLAGS} -std=c++17")

add_subdirectory(lib/Catch2)

set(SourceFiles 
    src/Coordinates/CoordConversions.cpp
    src/Coordinates/CoordContainerSkybox.cpp
    src/Coordinates/CoordContainerSpherical.cpp
    src/Coordinates/CoordContainerHemiSpherical.cpp
    src/Projections/EquirectangularProjection.cpp
    src/Projections/SkyboxProjection.cpp
    src/Projections/HemisphericalProjection.cpp
    src/Utils/EnvMapImage.cpp
    src/Utils/ImageReader.cpp
    src/Utils/ImageWriter.cpp
)

add_executable (envmapproj 
    src/main.cpp 
    ${SourceFiles}
)

add_executable (envmapproj_tests 
    tests/test_main.cpp 
    tests/ConversionTests.cpp
    tests/CoordContainerSkyboxTests.cpp
    tests/CoordConversionTests.cpp
    tests/CoordContainerSphericalTests.cpp
    tests/CoordContainerHemiSphericalTests.cpp
    tests/EnvMapImageTests.cpp 
    tests/EquirectangularProjectionTests.cpp
    tests/ImageReaderTests.cpp 
    tests/ImageWriterTests.cpp
    tests/SkyboxProjectionTests.cpp
    tests/HemisphericalProjectionTests.cpp
    ${SourceFiles}
)

target_link_libraries(envmapproj_tests PRIVATE Catch2::Catch2WithMain)

file(COPY assets DESTINATION bin)