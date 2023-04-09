cmake_minimum_required (VERSION 3.0.2)

project (EnvMapProjector)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

add_subdirectory(lib/Catch2)

set(SourceFiles 
    src/CoordsContainer2d.cpp
    src/ImageReader.cpp 
    src/ImageWriter.cpp 
    src/EnvMapImage.cpp 
    src/EquirectangularProjection.cpp
    src/SkyboxProjection.cpp
)

add_executable (envmapproj 
    src/main.cpp 
    ${SourceFiles}
)

add_executable (envmapproj_tests 
    tests/test_main.cpp 
    tests/ConversionTests.cpp
    tests/CoordsContainer2dTests.cpp
    tests/EnvMapImageTests.cpp 
    tests/EquirectangularProjectionTests.cpp 
    tests/ImageReaderTests.cpp 
    tests/ImageWriterTests.cpp
    tests/SkyboxProjectionTests.cpp
    ${SourceFiles}
)

target_link_libraries(envmapproj_tests PRIVATE Catch2::Catch2WithMain)
file(COPY assets DESTINATION bin)