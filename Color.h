#ifndef COLOR_H
#define COLOR_H

struct Color {
    int r, g, b;
};

Color getColor(double a);

// get color between c0 and c1
Color linColor(Color c0, Color c1, double a);

#endif