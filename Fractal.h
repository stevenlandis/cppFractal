#ifndef FRACTAL_H
#define FRACTAL_H

#include "Image.h"
#include <vector>
#include <string>

typedef std::vector<int> Base;

struct Fractal {
    const double padding = 0.1;

    struct Point {
        double x;
        double y;

        Point(double x, double y);
        Point();
        bool operator <(const Point &p) const;
    };

    Base base;
    int iterations;
    int d0;
    long long int turns;
    bool mirrored;
    std::vector<Point> points;
    std::vector<Point> boundary;
    std::string string;

    double min_x, min_y, max_x, max_y;

    Fractal(Base base, int iterations, bool mirrored);

    void setTurns();
    void setPoints();
    void setBoundary();
    void setBounds();
    void set_d0();

    void setString();
    void printPoints();
    void printBoundary();
    Image getImage(int width, int height);
    void saveImage(int width, int height);
};

#endif