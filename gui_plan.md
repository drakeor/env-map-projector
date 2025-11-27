Here's the high level description of the program:

Environment Map Projector is a free, open-source program that allows users to reproject thigns like Skyboxes to Equirectangular and vice-versa.```

Now, we want to convert from one projection type to another. Therefore, the program will be split into two parts: The input projection and output.

This is EquirectangularProjection, SkyboxProjection, and HemisphericalProjection. 

For input, notice that each one requires a different number of files That we need to select. We need to provide a nice gui that loads them and previews the images underneath. Then we select what coordinate system we want to export to. From here, we'll do some sanity checks too. That is, look at the image sizes. We'll have an option to autoscale some of the images to match either the highest or lowest resolution variant , or no scaling (selected by default).

Then it does the actual conversion. 

In the output will be 1 to 6 images (depending on the output type considered). The user can hit "save" which will save all the images to a directory of their choosing. 

Extra credit is a module that allows them to see it in 3D preview, but only if that's easy. Don't include if it's a lot of work. 

