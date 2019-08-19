#include "Fractal.h"
#include <iostream>

using namespace std;

int main() {
    cout << "starting\n";

    // Base base = {180, 30};
    // int a = 85;
    // Fractal fractal({90}, 19, true);
    // Fractal fractal({45,45,45,45}, 7, true);
    // Fractal fractal({36,36,36,36,36,36}, 6, true);
    // Fractal fractal({135,-135}, 13, false);
    // Fractal fractal({45,225},13,true);
    Fractal fractal({180,30},13,false);

    // fractal.printBoundary();
    // fractal.printPoints();

    Image img = fractal.getImage(44*60, 35*60);
    // img.erode();
    // Color c0 = {100,100,100};
    // Color c1 = {255,255,255};
    // img.smoothErode(c0,c1);
    img.save("out.bmp");
    // cout << "depth: " << depth << endl;

    // fractal.saveImage(2304, 1440); // Computer Resolution
    // fractal.saveImage(1334, 750); // phone resolution
    // fractal.saveImage(1920, 1080); // Desktop resolution
    // fractal.saveImage(750, 1334);
    // fractal.saveImage(100, 100);
}