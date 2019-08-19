# Fractal Generator (in c++)

![Faded Dragon Curve](color_fade.bmp)

This repository has a bunch of classes to generate fractals and save the designs to .bmp images.

This code is mainly for my personal use so it isn't too well commented. Good luck.

You can start with `main.cpp` as a starting point for how the code works. Running `make` should make an image called `out.bmp`.

If I want to make this more accesible, I'll make a website that makes it easy to create and save images.

The main logic for creating the fractal happens in `Fractal.cpp::Fractal::setPoints()`. It's pretty short and the rest of the code is for:
- positioning the fractal in the center of the image (requires convex hull of points)
- drawing the fractal to an image using lines.
- adding cool image effects.