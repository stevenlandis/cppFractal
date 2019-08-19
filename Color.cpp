#include "Color.h"

int rising(double v) {
    return (int)(255.0*v);
}

int falling(double v) {
    return rising(1-v);
}

Color getColor(double a) {
    a = a - static_cast<double>(static_cast<int>(a));

    Color res = {0,0,0};

    int section = (int)(6*a) % 6;
    double val = 6*a - section;

    switch(section) {
    case 0:
        res.r = 255;
        res.g = rising(val);
        res.b = 0;
        break;
    case 1:
        res.r = falling(val);
        res.g = 255;
        res.b = 0;
        break;
    case 2:
        res.r = 0;
        res.g = 255;
        res.b = rising(val);
        break;
    case 3:
        res.r = 0;
        res.g = falling(val);
        res.b = 255;
        break;
    case 4:
        res.r = rising(val);
        res.g = 0;
        res.b = 255;
        break;
    case 5:
        res.r = 255;
        res.g = 0;
        res.b = falling(val);
        break;
    }

    return res;
}

Color linColor(Color c0, Color c1, double a) {
    if (a > 1) a = 1;
    Color res;
    res.r = c0.r*(1-a) + c1.r*a;
    res.g = c0.g*(1-a) + c1.g*a;
    res.b = c0.b*(1-a) + c1.b*a;
    return res;
}