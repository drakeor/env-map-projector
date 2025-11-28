cmake_minimum_required (VERSION 3.10)

project (EnvMapProjector)

# Global output/config toggles
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "-O3 ${CMAKE_CXX_FLAGS}")
endif()

# Find vcpkg packages
find_package(Catch2 CONFIG REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)
find_package(Stb REQUIRED)

# Include Catch2 module for test discovery
include(CTest)
include(Catch)

# All reusable core sources live under src/
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

# Core library used by every executable/test target
add_library(envmapcore STATIC
    ${SourceFiles}
)
target_include_directories(envmapcore PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${Stb_INCLUDE_DIR}
)
target_link_libraries(envmapcore PUBLIC
    Eigen3::Eigen
)

# Catch2 unit/integration tests
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
)
target_link_libraries(envmapproj_tests PRIVATE envmapcore Catch2::Catch2WithMain)

catch_discover_tests(envmapproj_tests
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    PROPERTIES LABELS "unit"
)

# GUI dependencies
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_path(PORTABLE_FILE_DIALOGS_INCLUDE_DIRS "portable-file-dialogs.h")

# ImGui-based GUI program
add_executable(envmapproj_gui
    apps/gui/main.cpp
    apps/gui/GuiApp.cpp
    apps/gui/InputPanel.cpp
    apps/gui/OutputPanel.cpp
    apps/gui/ImageSlot.cpp
    apps/gui/ConversionController.cpp
    apps/gui/FileDialog.cpp
)
target_include_directories(envmapproj_gui PRIVATE
    ${PROJECT_SOURCE_DIR}/apps/gui
    ${PORTABLE_FILE_DIALOGS_INCLUDE_DIRS}
)
target_link_libraries(envmapproj_gui PRIVATE
    envmapcore
    glfw
    imgui::imgui
    OpenGL::GL
    ${CMAKE_DL_LIBS}
)

# Sample no-GUI program that uses the pure core library
add_executable(envmapproj
    apps/nogui/main.cpp
)
target_link_libraries(envmapproj PRIVATE envmapcore)

# Add assets to the output directory
file(COPY assets DESTINATION bin)

# Install targets for the gui and nogui apps, along with assets
install(TARGETS envmapproj_gui envmapproj
    RUNTIME DESTINATION bin
)
install(TARGETS envmapproj
    RUNTIME DESTINATION bin
)
install(DIRECTORY assets DESTINATION bin)
