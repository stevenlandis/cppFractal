#include "Image.h"
#include "Color.h"
#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

struct Point {
    int x, y;
};

// ------------------
//  Helper Functions
// ------------------
const int bytesPerPixel = 3; /// red, green, blue
const int fileHeaderSize = 14;
const int infoHeaderSize = 40;

void generateBitmapImage(unsigned char *image, int height, int width, const char* imageFileName);
unsigned char* createBitmapFileHeader(int height, int width);
unsigned char* createBitmapInfoHeader(int height, int width);

void generateBitmapImage(unsigned char *image, int height, int width, const char* imageFileName){

    unsigned char* fileHeader = createBitmapFileHeader(height, width);
    unsigned char* infoHeader = createBitmapInfoHeader(height, width);
    unsigned char padding[3] = {0, 0, 0};
    int paddingSize = (4-(width*bytesPerPixel)%4)%4;

    FILE* imageFile = fopen(imageFileName, "wb");

    fwrite(fileHeader, 1, fileHeaderSize, imageFile);
    fwrite(infoHeader, 1, infoHeaderSize, imageFile);

    int i;
    for(i=0; i<height; i++){
        fwrite(image+(i*width*bytesPerPixel), bytesPerPixel, width, imageFile);
        fwrite(padding, 1, paddingSize, imageFile);
    }

    fclose(imageFile);
}

unsigned char* createBitmapFileHeader(int height, int width){
    int fileSize = fileHeaderSize + infoHeaderSize + bytesPerPixel*height*width;

    static unsigned char fileHeader[] = {
        0,0, /// signature
        0,0,0,0, /// image file size in bytes
        0,0,0,0, /// reserved
        0,0,0,0, /// start of pixel array
    };

    fileHeader[ 0] = (unsigned char)('B');
    fileHeader[ 1] = (unsigned char)('M');
    fileHeader[ 2] = (unsigned char)(fileSize    );
    fileHeader[ 3] = (unsigned char)(fileSize>> 8);
    fileHeader[ 4] = (unsigned char)(fileSize>>16);
    fileHeader[ 5] = (unsigned char)(fileSize>>24);
    fileHeader[10] = (unsigned char)(fileHeaderSize + infoHeaderSize);

    return fileHeader;
}

unsigned char* createBitmapInfoHeader(int height, int width){
    static unsigned char infoHeader[] = {
        0,0,0,0, /// header size
        0,0,0,0, /// image width
        0,0,0,0, /// image height
        0,0, /// number of color planes
        0,0, /// bits per pixel
        0,0,0,0, /// compression
        0,0,0,0, /// image size
        0,0,0,0, /// horizontal resolution
        0,0,0,0, /// vertical resolution
        0,0,0,0, /// colors in color table
        0,0,0,0, /// important color count
    };

    infoHeader[ 0] = (unsigned char)(infoHeaderSize);
    infoHeader[ 4] = (unsigned char)(width    );
    infoHeader[ 5] = (unsigned char)(width>> 8);
    infoHeader[ 6] = (unsigned char)(width>>16);
    infoHeader[ 7] = (unsigned char)(width>>24);
    infoHeader[ 8] = (unsigned char)(height    );
    infoHeader[ 9] = (unsigned char)(height>> 8);
    infoHeader[10] = (unsigned char)(height>>16);
    infoHeader[11] = (unsigned char)(height>>24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(bytesPerPixel*8);

    return infoHeader;
}

// -------
//  Image
// -------
Image::Image(int width, int height)
: width(width), height(height) {
    image = new unsigned char[width*height*3];
}

Image::~Image() {
    delete[] image;
}

void Image::setPixel(int x, int y, int c) {
    image[2 + 3*(x + width * y)] = c; // r
    image[1 + 3*(x + width * y)] = c; // g
    image[0 + 3*(x + width * y)] = c; // b
}

void Image::setPixel(int x, int y, Color c) {
    image[2 + 3*(x + width * y)] = c.r; // r
    image[1 + 3*(x + width * y)] = c.g; // g
    image[0 + 3*(x + width * y)] = c.b; // b
}

void Image::setBackground(int c) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            setPixel(x, y, c);
        }
    }
}

void Image::line(int x0, int y0, int x1, int y1) {
    int c = 110;

    // limit line to boundary
    assert(x0 >= 0 && x0 < width);
    assert(y0 >= 0 && y0 < height);
    assert(x1 >= 0 && x1 < width);
    assert(y1 >= 0 && y1 < height);

    // cout<<"("<<x0<<", "<<y0<<") -> ("<<x1<<", "<<y1<<")\n";

    int x,y,dx,dy,dx0,dy0,px,py,xe,ye,i;
    dx=x1-x0;
    dy=y1-y0;
    dx0=abs(dx);
    dy0=abs(dy);
    px=2*dy0-dx0;
    py=2*dx0-dy0;
    if (dy0<=dx0) {
        if (dx>=0) {
            x=x0;
            y=y0;
            xe=x1;
        } else {
            x=x1;
            y=y1;
            xe=x0;
        }
        setPixel(x,y,c);
        for (i=0;x<xe;i++) {
            x=x+1;
            if (px<0) {
                px=px+2*dy0;
            } else {
                if ((dx<0 && dy<0) || (dx>0 && dy>0)) {
                    y=y+1;
                } else {
                    y=y-1;
                }
                px=px+2*(dy0-dx0);
            }
            setPixel(x,y,c);
        }
    }
    else
    {
        if(dy>=0)
        {
            x=x0;
            y=y0;
            ye=y1;
        }
        else
        {
            x=x1;
            y=y1;
            ye=y0;
        }
        setPixel(x,y,c);
        for(i=0;y<ye;i++)
        {
            y=y+1;
            if(py<=0)
            {
                py=py+2*dx0;
            }
            else
            {
                if((dx<0 && dy<0) || (dx>0 && dy>0))
                {
                    x=x+1;
                }
                else
                {
                    x=x-1;
                }
                py=py+2*(dx0-dy0);
            }
            setPixel(x,y,c);
        }
    }
}

void Image::line(Color c, int x0, int y0, int x1, int y1) {
    // limit line to boundary
    assert(x0 >= 0 && x0 < width);
    assert(y0 >= 0 && y0 < height);
    assert(x1 >= 0 && x1 < width);
    assert(y1 >= 0 && y1 < height);

    // cout<<"("<<x0<<", "<<y0<<") -> ("<<x1<<", "<<y1<<")\n";

    int x,y,dx,dy,dx0,dy0,px,py,xe,ye,i;
    dx=x1-x0;
    dy=y1-y0;
    dx0=abs(dx);
    dy0=abs(dy);
    px=2*dy0-dx0;
    py=2*dx0-dy0;
    if (dy0<=dx0) {
        if (dx>=0) {
            x=x0;
            y=y0;
            xe=x1;
        } else {
            x=x1;
            y=y1;
            xe=x0;
        }
        setPixel(x,y,c);
        for (i=0;x<xe;i++) {
            x=x+1;
            if (px<0) {
                px=px+2*dy0;
            } else {
                if ((dx<0 && dy<0) || (dx>0 && dy>0)) {
                    y=y+1;
                } else {
                    y=y-1;
                }
                px=px+2*(dy0-dx0);
            }
            setPixel(x,y,c);
        }
    }
    else
    {
        if(dy>=0)
        {
            x=x0;
            y=y0;
            ye=y1;
        }
        else
        {
            x=x1;
            y=y1;
            ye=y0;
        }
        setPixel(x,y,c);
        for(i=0;y<ye;i++)
        {
            y=y+1;
            if(py<=0)
            {
                py=py+2*dx0;
            }
            else
            {
                if((dx<0 && dy<0) || (dx>0 && dy>0))
                {
                    x=x+1;
                }
                else
                {
                    x=x-1;
                }
                py=py+2*(dx0-dy0);
            }
            setPixel(x,y,c);
        }
    }
}

void Image::save(string fName) {
    generateBitmapImage(image, height, width, fName.c_str());
}

void Image::downsample(int n) {
    int newWidth = width/n;
    int newHeight = height/n;
    unsigned char* newImage = new unsigned char[newWidth*newHeight*3];

    for (int x = 0; x < newWidth; x++) {
        for (int y = 0; y < newHeight; y++) {
            int r = 0;
            int g = 0;
            int b = 0;

            // loop through n x n square
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    int xx = x*n + i;
                    int yy = y*n + j;

                    r += image[2 + 3*(xx + width*yy)];
                    g += image[1 + 3*(xx + width*yy)];
                    b += image[0 + 3*(xx + width*yy)];
                }
            }

            newImage[2 + 3*(x + newWidth*y)] = r/n/n;
            newImage[1 + 3*(x + newWidth*y)] = g/n/n;
            newImage[0 + 3*(x + newWidth*y)] = b/n/n;
        }
    }

    delete[] image;
    image = newImage;
    width = newWidth;
    height = newHeight;
}

void Image::erode() {
    unsigned char* newImage = new unsigned char[width*height*3];
    for (int i = 0; i < width*height*3; i++) {
        newImage[i] = 200;
    }

    vector<Point> stack;
    bool run = true;
    double depth = 0.0;
    int depthI = 0;
    while (run) {
        // get all boundary points
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                if (
                        image[3*(x + width*y)] == 0 &&
                        !(
                            x > 0        && image[3*((x-1) + width*(y))] == 0 &&
                            y > 0        && image[3*((x) + width*(y-1))] == 0 &&
                            x+1 < width  && image[3*((x+1) + width*(y))] == 0 &&
                            y+1 < height && image[3*((x) + width*(y+1))] == 0
                        )) {
                    stack.push_back({x,y});
                }
            }
        }
        // cout << stack.size() << ", " << (width*height) << endl;
        run = (stack.size() != 0);
        Color c = getColor(depth);
        while (stack.size()) {
            Point p = stack.back();
            stack.pop_back();
            newImage[2 + 3*(p.x + width*p.y)] = c.r;
            newImage[1 + 3*(p.x + width*p.y)] = c.g;
            newImage[0 + 3*(p.x + width*p.y)] = c.b;

            image[2 + 3*(p.x + width*p.y)] = 255;
            image[1 + 3*(p.x + width*p.y)] = 255;
            image[0 + 3*(p.x + width*p.y)] = 255;
        }
        depthI++;
        if (depthI % 10 == 9) {
            depth += 0.1;
        }
    }

    delete[] image;
    image = newImage;
}

// image still needs to be black and white
int Image::getErodeDepth() {
    // copy the current black and white image
    unsigned char* imgCopy = new unsigned char[width*height*3];
    for (int i = 0; i < width*height*3; i++) {
        imgCopy[i] = image[i];
    }

    vector<Point> stack;
    bool run = true;
    int depthI = 0;
    while (run) {
        // get all boundary points
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                if (
                        imgCopy[3*(x + width*y)] == 0 &&
                        !(
                            x > 0        && imgCopy[3*((x-1) + width*(y))] == 0 &&
                            y > 0        && imgCopy[3*((x) + width*(y-1))] == 0 &&
                            x+1 < width  && imgCopy[3*((x+1) + width*(y))] == 0 &&
                            y+1 < height && imgCopy[3*((x) + width*(y+1))] == 0
                        )) {
                    stack.push_back({x,y});
                }
            }
        }
        // cout << stack.size() << ", " << (width*height) << endl;
        run = (stack.size() != 0);
        while (stack.size()) {
            Point p = stack.back();
            stack.pop_back();

            imgCopy[2 + 3*(p.x + width*p.y)] = 255;
            imgCopy[1 + 3*(p.x + width*p.y)] = 255;
            imgCopy[0 + 3*(p.x + width*p.y)] = 255;
        }
        depthI++;
    }

    delete[] imgCopy;

    return depthI-1;
}

// fades from background: c0
// to center: c1
void Image::smoothErode(Color c0, Color c1) {
    int depth = getErodeDepth()+1;

    unsigned char* newImage = new unsigned char[width*height*3];
    for (int i = 0; i < width*height*3; i+=3) {
        newImage[i+0] = c0.r;
        newImage[i+1] = c0.g;
        newImage[i+2] = c0.b;
    }

    vector<Point> stack;
    int stepSize = 10;
    bool run = true;
    int depthCount = 0;
    double depthI = 0;
    double depthDelta = static_cast<double>(stepSize)/static_cast<double>(depth);
    while (run) {
        // get all boundary points
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                if (
                        image[3*(x + width*y)] == 0 &&
                        !(
                            x > 0        && image[3*((x-1) + width*(y))] == 0 &&
                            y > 0        && image[3*((x) + width*(y-1))] == 0 &&
                            x+1 < width  && image[3*((x+1) + width*(y))] == 0 &&
                            y+1 < height && image[3*((x) + width*(y+1))] == 0
                        )) {
                    stack.push_back({x,y});
                }
            }
        }
        // cout << stack.size() << ", " << (width*height) << endl;
        run = (stack.size() != 0);
        Color c = linColor(c0, c1, depthI);
        while (stack.size()) {
            Point p = stack.back();
            stack.pop_back();
            newImage[2 + 3*(p.x + width*p.y)] = c.r;
            newImage[1 + 3*(p.x + width*p.y)] = c.g;
            newImage[0 + 3*(p.x + width*p.y)] = c.b;

            image[2 + 3*(p.x + width*p.y)] = 255;
            image[1 + 3*(p.x + width*p.y)] = 255;
            image[0 + 3*(p.x + width*p.y)] = 255;
        }
        if ((depthCount) % stepSize == 0) {
            depthI += depthDelta;
        }
        depthCount++;
    }

    delete[] image;
    image = newImage;
}