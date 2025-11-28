
# Environment Map Projector
[![Environment Map Projector](https://github.com/drakeor/env-map-projector/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/drakeor/env-map-projector/actions/workflows/cmake-multi-platform.yml) [![CodeQL Advanced](https://github.com/drakeor/env-map-projector/actions/workflows/codeql.yml/badge.svg)](https://github.com/drakeor/env-map-projector/actions/workflows/codeql.yml)

Environment Map Projector is a free, cross-platform open-source program that allows users to convert (re-project) things like Skyboxes to Equirectangular format and vice-versa. It's mostly used for game engines and 3D renderers like Unity, Stride, Unreal Engine, etc.

There are a few tools available already to do this task. However, many of them are locked behind either commercial software, or paid assets in the Unity Store. There are few implementations on GitHub that are written in Python, but I've found these to run incredibly slow when given high resolution textures.

![conversion_image](https://drakeor.com/uploads/envmapproj_convert.png)


## Features

* Support for conversions between Equirectangular, Skybox, and Hemispherical/Skydome projections (Note that projection from Skydome is currently a bit buggy and may produce artifacts).

* Can theoretically handle extremely large images. (I've only tested up to 16k).

* Cross-platform, efficient, portable (No additional libraries/programs needed)

* No ads, bloat, etc; It's completely free!

## Demo

![demo](https://drakeor.com/uploads/envmapprojector-demo.gif)


## Getting Started

For users just wanting to download a working tool, we recommend getting the latest release from the Releases tab. These versus are generally more stable than what's on the master branch.

[[Download the Latest Release](https://github.com/drakeor/env-map-projector/releases/)]

![interface](https://drakeor.com/uploads/envmapproject-screenshot.png)

The interface should be fairly straightforward:

1) Choose your input projection type on the left and select the images you want to convert from. I've provided some sample assets to experiment with in the ```assets/``` folder.

2) Choose your output projection type on the right and click convert. By default, the converted files will be under the ```output/``` folder.

3) That's it!

The additional options you see are described below:

- Input Scaling: This is only useful in the case that one of your images are not the same size (i.e. for Skybox). The program will attempt to automatically scale the others to match sizes.

- UI scale: This sets the scale of the interface in case the elements are too small/big.

- Filename prefix: What string to prefix the converted files with.

- Output directory: Where to save the converted images

## Using the core library

The core library is stored under src/. The only dependency is eigen (For vector math) and stb (for image loading). A sample program (also located in ```apps/nogui/main.cpp```) is provided below:

```cpp
#include "Projections/SkyboxProjection.h"
#include "Projections/EquirectangularProjection.h"
#include "Projections/HemisphericalProjection.h"
#include "Utils/ImageReader.h"
#include "Utils/ImageWriter.h"

using namespace EnvProj;

int main()
{
    // Load an equirectangular image from the filesystem
    ImageReader reader;
    auto srcImg = reader.LoadImage("assets/testimages/equirectangular_image.jpg");

    EquirectangularProjection<double> sourceProj;
    auto coords = sourceProj.LoadImageToSphericalCoords(&srcImg);
    ImageWriter writer;

    // Convert to a skybox projection
    SkyboxProjection<double> destProj;
    std::array<EnvMapImage, 6> destImgs = destProj.ConvertToImages(coords.get(), 2048);
    
    writer.SaveImage("assets/testoutput/huge_converted_skybox_top.png", destImgs[0]);
    writer.SaveImage("assets/testoutput/huge_converted_skybox_bottom.png", destImgs[1]);
    writer.SaveImage("assets/testoutput/huge_converted_skybox_left.png", destImgs[2]);
    writer.SaveImage("assets/testoutput/huge_converted_skybox_right.png", destImgs[3]);
    writer.SaveImage("assets/testoutput/huge_converted_skybox_front.png", destImgs[4]);
    writer.SaveImage("assets/testoutput/huge_converted_skybox_back.png", destImgs[5]);

    // Convert to a skydome/hemispherical projection
    HemisphericalProjection<double> destProj2;
    std::array<EnvMapImage, 2> destImgs2 = destProj2.ConvertToImages(coords.get(), 2048);
    
    writer.SaveImage("assets/testoutput/huge_converted_hemi_top.png", destImgs2[0]);
    writer.SaveImage("assets/testoutput/huge_converted_hemi_bottom.png", destImgs2[1]);

    return 0;
}
```

A complete guide to the theory behind the code can be found on [my blog](https://drakeor.com/2023/04/27/equirectangular-to-skybox-projection/). I encourage you to read it if you are interested in the math and algorithms behind it. It should be accessible if you've had at least a little exposure to linear algebra.

## Building from source

### Pre-requisites

Note this repository has a copy of all the libraries (with the version I used) inside the ```lib/``` folder for convenience. Beyond that, you need the following:

- CMAKE
- GLFW (```apt-get install libglfw-dev``` on linux)
- OpenGL libraries

### Building

```
cmake .
make -j 8
```

The executables will be under ```bin/``` following successful compilation.

## Libraries used:

* STB for image loading: https://github.com/nothings/stb
* Catch2 for unit testing: https://github.com/catchorg/Catch2
* Eigen for Linear Algebra operations: https://gitlab.com/libeigen/eigen
* Dear IMGUI for the front end interface: https://github.com/ocornut/imgui
* GLFW for rendering the front end: https://www.glfw.org
* Portable File Dialogs for well, the file dialogs: https://github.com/samhocevar/portable-file-dialogs


Note: I know the default convention is to either submodule them, use fetch content in CMake, or tell you to install them, but I've been burned by that when cloning other libraries before and it's annoying to fix.

## Credits:

* Eric Christian (@Raygoe) for his insight and improvements on the C++ side. 

* (LearnOpenGl.com)[https://learnopengl.com/Advanced-OpenGL/Cubemaps] which is where I got the Skybox from

* (Timothy Oldfield)[https://unsplash.com/@oldfieldart] for the Equirectangular texture.

* (Tutorials for Blender3d)[https://tutorialsforblender3d.com/] for the skysphere texture.

* (Solar System Scope)[https://www.solarsystemscope.com/textures/] for 8k texture tests.


### License

MIT License