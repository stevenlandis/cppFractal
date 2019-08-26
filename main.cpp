#include "Fractal.h"
#include <iostream>

using namespace std;

int main() {
    cout << "starting\n";

    // Base base = {180, 30};
    // int a = 85;
    // Fractal fractal({90}, 21, true);
    // Fractal fractal({45,45,45,45}, 7, true);
    // Fractal fractal({36,36,36,36,36,36}, 7, true);
    // Fractal fractal({135,-135}, 13, false);
    // Fractal fractal({45,225},13,true);
    // Fractal fractal({180,30},13,false);
    // Fractal fractal({90, 90},13,true);
    // Fractal fractal({45, 180},13,false);
    // Fractal fractal({45, 180},13,false);
    // Fractal fractal({90, -45},15,false);
    // Fractal fractal({135,-135,90},7,false);
    // Fractal fractal({135,-135,90},7,false);
    // Fractal fractal({108}, 23, true);
    // Fractal fractal({45,-90,45,-135}, 8, true);
    Fractal fractal({60,-120,60,60}, 10, false);

    // fractal.printBoundary();
    // fractal.printPoints();

    Image img = fractal.getImage(44*60, 35*60, {0, 0, 0}, getPrimaryColor);
    // Image img = fractal.getImage(44*60, 35*60);
    // img.erode();
    // Color c0 = {100,100,100};
    // Color c1 = {255,255,255};
    // img.smoothErode(c0,c1);
    string name = fractal.string + ".bmp";
    img.save(name);
    // img.save("out.bmp");
    // cout << "depth: " << depth << endl;

    // fractal.saveImage(2304, 1440); // Computer Resolution
    // fractal.saveImage(1334, 750); // phone resolution
    // fractal.saveImage(1920, 1080); // Desktop resolution
    // fractal.saveImage(750, 1334);
    // fractal.saveImage(100, 100);
}