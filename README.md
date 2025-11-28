
# Environment Map Projector
[![Environment Map Projector](https://github.com/drakeor/env-map-projector/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/drakeor/env-map-projector/actions/workflows/cmake-multi-platform.yml)

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

### Prerequisites

All library dependencies (Catch2, Eigen3, GLFW, ImGui, STB, Portable File Dialogs) are managed by [vcpkg](https://github.com/microsoft/vcpkg) and will be automatically downloaded and built during the CMake configuration phase.

**Common Requirements (All Platforms):**
- **CMake** 3.21 or higher (required for CMakePresets.json support)
- **vcpkg** - Microsoft's C++ package manager ([installation guide](https://github.com/microsoft/vcpkg#get-started))
- **C++17 compatible compiler**

**Platform-Specific Requirements:**

<details>
<summary><b>Linux (Ubuntu/Debian)</b></summary>

**Compiler:** GCC 7+ or Clang 5+

**System Dependencies:**
```bash
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    pkg-config \
    ninja-build \
    libglfw3-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libxinerama-dev \
    libxcursor-dev \
    xorg-dev
```

**vcpkg Setup:**
```bash
# Clone vcpkg if you haven't already
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg && ./bootstrap-vcpkg.sh
export VCPKG_ROOT=$(pwd)
```
</details>

<details>
<summary><b>macOS (Apple Silicon & Intel)</b></summary>

**Compiler:** Apple Clang 12+ (Xcode Command Line Tools)

**System Dependencies:**
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install Homebrew dependencies
brew install ninja pkg-config autoconf automake libtool
```

**vcpkg Setup:**
```bash
# Clone vcpkg if you haven't already
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg && ./bootstrap-vcpkg.sh
export VCPKG_ROOT=$(pwd)
```
</details>

<details>
<summary><b>Windows</b></summary>

**Compiler:** Visual Studio 2019 or newer (MSVC 19.20+)

**Required Components:**
- Visual Studio with "Desktop development with C++" workload
- Windows 10 SDK

**vcpkg Setup:**
```powershell
# Clone vcpkg if you haven't already
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
$env:VCPKG_ROOT = (Get-Location).Path
```

**Note:** You can also use the Visual Studio Developer Command Prompt which has all necessary tools pre-configured.
</details>

### Building

Configure, Build, Test, and Deploy with

```bash
cmake --preset release
cmake --build --preset release
ctest --preset release --verbose
cmake --install build/release --prefix install-output
```

The executables will be under `install-output/bin/` following successful install.

## Libraries used:

* STB for image loading: https://github.com/nothings/stb
* Catch2 for unit testing: https://github.com/catchorg/Catch2
* Eigen for Linear Algebra operations: https://gitlab.com/libeigen/eigen
* Dear IMGUI for the front end interface: https://github.com/ocornut/imgui
* GLFW for rendering the front end: https://www.glfw.org
* Portable File Dialogs for well, the file dialogs: https://github.com/samhocevar/portable-file-dialogs

Note: These are provided by the [vcpkg](https://github.com/microsoft/vcpkg?tab=readme-ov-file#get-started) project which is a requirement to build on most build systems these days so you probably already have it (along with CMake of course).

## Credits:

* Eric Christian (@Raygoe) for his insight and improvements on the C++ side. 

* [LearnOpenGl.com](https://learnopengl.com/Advanced-OpenGL/Cubemaps) which is where I got the Skybox from

* [Timothy Oldfield](https://unsplash.com/@oldfieldart) for the Equirectangular texture.

* [Tutorials for Blender3d](https://tutorialsforblender3d.com/) for the skysphere texture.

* [Solar System Scope](https://www.solarsystemscope.com/textures/) for 8k texture tests.


### License

MIT License