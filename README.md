
# Environment Map Projector

Environment Map Projector is a free, open-source program that allows users to reproject thigns like Skyboxes to Equirectangular and vice-versa.

There are a few tools available already to do this task. However, some are locked behind either commercial software or paid assets in the Unity Store. There are few implementations on GitHub that are written in Python, but these run pretty slow.

![conversion_image](https://drakeor.com/uploads/envmapproj_convert.png)

## Features

This Program currently supports:
* Equirectangular Projections
* Skybox Projections
* Hemispherical (Skydome) Projections

## Usage

This application is still under development and I have yet to write a front-end for it. You can see a sample program in main.cpp.

## Documentation

Program Documentation (Coming soon)
Theory behind Equirectangular Projection (Coming soon)
CubeMap to Equirectangular Projection (Coming soon)

## Credits:

* Eric Christian (@Raygoe) for his insight and improvements on the C++ side. 

* (LearnOpenGl.com)[https://learnopengl.com/Advanced-OpenGL/Cubemaps] which is where I got the Skybox from

* (Timothy Oldfield)[https://unsplash.com/@oldfieldart] for the Equirectangular texture.

* (Tutorials for Blender3d)[https://tutorialsforblender3d.com/] for the skysphere texture.

* (Solar System Scope)[https://www.solarsystemscope.com/textures/] for 8k texture tests.

## Libraries used:

* STB for image loading: https://github.com/nothings/stb
* Catch2 for Unit Testing: https://github.com/catchorg/Catch2
* Eigen for Linear Algebra operations: https://gitlab.com/libeigen/eigen

Note this repository has a copy of all the libraries (with the version I used) attached.

I know the default convention is to either submodule them or tell you to install them, but I've been burned by that when cloning other libraries and it's annoying to fix.

### License

MIT License