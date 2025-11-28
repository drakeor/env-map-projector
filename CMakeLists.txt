cmake_minimum_required (VERSION 3.5.2)

project (EnvMapProjector)

# Global output/config toggles
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_CXX_FLAGS "-O3 ${CMAKE_CXX_FLAGS} -std=c++17")

# Add Catch2 as a subdirectory for testing
add_subdirectory(lib/catch2)

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
    ${PROJECT_SOURCE_DIR}/lib/eigen
    ${PROJECT_SOURCE_DIR}/lib/stb
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

# ImGui sources
set(IMGUI_DIR ${PROJECT_SOURCE_DIR}/lib/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# GUI dependencies
find_package(OpenGL REQUIRED)

# Prefer an exported GLFW target; fallback to pkg-config otherwise
set(GLFW_TARGET "")
set(GLFW_LINK_LIBS "")
set(GLFW_INCLUDE_DIRS "")

find_package(glfw3 CONFIG QUIET)
if(TARGET glfw)
    set(GLFW_TARGET glfw)
elseif(TARGET glfw::glfw)
    set(GLFW_TARGET glfw::glfw)
elseif(glfw3_FOUND)
    set(GLFW_TARGET glfw)
else()
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(GLFW REQUIRED glfw3)
        set(GLFW_LINK_LIBS ${GLFW_LIBRARIES})
        set(GLFW_INCLUDE_DIRS ${GLFW_INCLUDE_DIRS})
    else()
        message(FATAL_ERROR "GLFW not found. Install glfw3 or provide a pkg-config file.")
    endif()
endif()

# ImGui-based GUI program
add_executable(envmapproj_gui
    apps/gui/main.cpp
    apps/gui/GuiApp.cpp
    apps/gui/InputPanel.cpp
    apps/gui/OutputPanel.cpp
    apps/gui/ImageSlot.cpp
    apps/gui/ConversionController.cpp
    apps/gui/FileDialog.cpp
    ${IMGUI_SOURCES}
)
target_include_directories(envmapproj_gui PRIVATE
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${PROJECT_SOURCE_DIR}/apps/gui
    ${PROJECT_SOURCE_DIR}/lib/portable-file-dialogs
    ${GLFW_INCLUDE_DIRS}
)

# Link to GLFW appropriately based on how it was found
if(GLFW_TARGET STREQUAL "")
    target_link_libraries(envmapproj_gui PRIVATE
        envmapcore
        ${GLFW_LINK_LIBS}
        OpenGL::GL
        ${CMAKE_DL_LIBS}
    )
else()
    target_link_libraries(envmapproj_gui PRIVATE
        envmapcore
        ${GLFW_TARGET}
        OpenGL::GL
        ${CMAKE_DL_LIBS}
    )
endif()

# Sample no-GUI program that uses the pure core library
add_executable(envmapproj
    apps/nogui/main.cpp
)
target_link_libraries(envmapproj PRIVATE envmapcore)

# Add assets to the output directory
file(COPY assets DESTINATION bin)