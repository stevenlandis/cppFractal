#ifndef IMAGE_H
#define IMAGE_H

#include "Color.h"
#include <string>

struct Image {
    unsigned char* image;
    int width;
    int height;

    Image(int width, int height);
    ~Image();

    void setPixel(int x, int y, int c);
    void setPixel(int x, int y, Color c);
    void setBackground(int c);
    void setBackground(Color c);
    void line(int x0, int y0, int x1, int y1);
    void line(Color c, int x0, int y0, int x1, int y1);

    void downsample(int n);
    void erode();
    int getErodeDepth();
    void smoothErode(Color c0, Color c1);
    void save(std::string fName);
};

#endif