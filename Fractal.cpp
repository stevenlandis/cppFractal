#include "Fractal.h"
#include "Image.h"
#include <iostream>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <limits>
#include <sstream>

using namespace std;
typedef Fractal::Point Point;

const double INF = numeric_limits<double>::infinity();
const double PI = 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899;

// ------------------
//  Helper Functions
// ------------------
int positive_modulo(int i, int n) {
    return (i % n + n) % n;
}

double cross(const Point &O, const Point &A, const Point &B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

vector<Point> convex_hull(vector<Point> P) {
    size_t n = P.size(), k = 0;
    if (n <= 3) return P;
    vector<Point> H(2*n);

    // Sort points lexicographically
    sort(P.begin(), P.end());

    // Build lower hull
    for (size_t i = 0; i < n; ++i) {
        while (k >= 2 && cross(H[k-2], H[k-1], P[i]) <= 0) k--;
        H[k++] = P[i];
    }

    // Build upper hull
    for (size_t i = n-1, t = k+1; i > 0; --i) {
        while (k >= t && cross(H[k-2], H[k-1], P[i-1]) <= 0) k--;
        H[k++] = P[i-1];
    }

    H.resize(k-1);
    return H;
}

// -------
//  Point
// -------
Point::Point(double x, double y)
: x(x), y(y)
{}

Point::Point() {}

bool Point::operator <(const Point &p) const {
    return x < p.x || (x == p.x && y < p.y);
}

// ---------
//  Fractal
// ---------
Fractal::Fractal(Base base, int iterations, bool mirrored)
: base(base), iterations(iterations), mirrored(mirrored)
{
    d0 = 0;
    setString();
    cout << "Name: " << string << endl;
    setTurns();
    setPoints();
    setBoundary();
    set_d0();
    setPoints();
    setBoundary();
    setBounds();
}

void Fractal::setTurns() {
    int bl = base.size();
    turns = bl;
    for (int i = 0; i < iterations; i++) {
        turns = bl + bl*turns + turns;
    }

    cout << "Fractal has " << turns << " turns\n";
}

void Fractal::setPoints() {
    points.clear();

    int dir = d0;
    points.push_back({0,0});
    double x = 0;
    double y = 0;

    double sinTable[360];
    double cosTable[360];
    for (int i = 0; i < 360; i++) {
        sinTable[i] = sin(((double)i)*PI/180);
        cosTable[i] = cos(((double)i)*PI/180);
    }

    int baseSize = base.size() + 1;
    for (int i = 1; i <= turns; i++) {
        int turnN = i;
        while (turnN % baseSize == 0) {
            turnN /= baseSize;
        }
        if (mirrored) {
            if ((turnN/baseSize) % 2 == 0) {
                dir += base[turnN % baseSize - 1];
            } else {
                dir -= base[baseSize - 1 - turnN % baseSize];
            }
        } else {
            dir += base[turnN % baseSize - 1];
        }

        dir = positive_modulo(dir, 360);

        x += cosTable[dir];
        y += sinTable[dir];

        points.push_back({x, y});

        // cout << "Direction: " << dir << endl;
    }

    // printPoints();
}

void Fractal::setBoundary() {
    boundary = convex_hull(points);

    cout << "There are " << boundary.size() << " boundary points\n";
    // printBoundary();
}

void Fractal::setBounds() {
    min_x = min_y =  INF;
    max_x = max_y = -INF;
    for (auto point : points) {
        if (point.x < min_x) min_x = point.x;
        if (point.y < min_y) min_y = point.y;
        if (point.x > max_x) max_x = point.x;
        if (point.y > max_y) max_y = point.y;
    }
}

void Fractal::set_d0() {
    int nb = boundary.size();

    double minDotRange = INF;
    int minDotI;

    for (int i = 0; i < nb; i++) {
        Point p0 = boundary[i];
        Point p1 = boundary[(i+1)%nb];

        double minDot = INF;
        double maxDot = -INF;

        double dx = p1.x - p0.x;
        double dy = p1.y - p0.y;

        double l = sqrt(dx*dx + dy*dy);

        // get x and y parts of perpendicular unit vector
        double ux = -dy/l;
        double uy =  dx/l;

        // get min and max dot product between u and each point
        for (int j = 0; j < nb; j++) {
            double dot = boundary[j].x * ux + boundary[j].y * uy;

            if (dot < minDot) minDot = dot;
            if (dot > maxDot) maxDot = dot;
        }

        double dotRange = maxDot - minDot;
        if (dotRange < minDotRange) {
            minDotRange = dotRange;
            minDotI = i;
        }
    }

    // cout << minDotI << " is min edge with range " << minDotRange << endl;

    // now, calculate the angle of optimal edge
    Point p0 = boundary[minDotI];
    Point p1 = boundary[(minDotI+1)%nb];

    double dx = p1.x - p0.x;
    double dy = p1.y - p0.y;

    double angle = atan2(dy, dx);
    d0 = positive_modulo(-180*angle/PI + 0.5, 360);

    cout << "optimal angle: " << d0 << endl;
}

void Fractal::setString() {
    ostringstream os;
    os << "[";
    if (base.size() > 0) {
        os << base[0];
        for (unsigned int i = 1; i < base.size(); i++) {
            os << "," << base[i];
        }
    }
    os << "], ";

    os << (mirrored ? "true" : "false");

    os << ", " << iterations;

    string = os.str();
}

void Fractal::printPoints() {
    ofstream of("points.txt");

    for (auto point : points) {
        of << point.x << " " << point.y << endl;
    }

    of.close();
}

void Fractal::printBoundary() {
    ofstream of("boundary.txt");

    for (auto point : boundary) {
        of << point.x << " " << point.y << endl;
    }

    of.close();
}

Image Fractal::getImage(int width, int height) {
    Image img(width, height);
    img.setBackground(0);

    // get size of fractal
    double wf = max_x - min_x;
    double hf = max_y - min_y;

    // get image width and height as doubles
    double wi = width;
    double hi = height;

    double scale, shift_x, shift_y;

    if (hf*wi > hi*wf) {
        // vertical constraint
        scale = hi/hf * (1-2*padding);
        shift_y = hi*padding - scale*min_y;
        shift_x = (wi - scale*(min_x+max_x))/2;
    } else {
        // horizontal constraint
        scale = wi/wf * (1-2*padding);
        shift_x = wi*padding - scale*min_x;
        shift_y = (hi - scale*(min_y+max_y))/2; 
    }

    cout << "scale: " << scale
         << ", shift x: " << shift_x
         << ", shift_y: " << shift_y << endl;

    // cout << points.size() << " points, " << turns << " turns\n";

    for (int i = 0; i < turns; i++) {
        Color c = getColor(static_cast<double>(i)/turns);
        // Color c = {0,0,0};

        img.line(
            c,
            (int)(0.5 + points[i  ].x * scale + shift_x),
            (int)(0.5 + points[i  ].y * scale + shift_y),
            (int)(0.5 + points[i+1].x * scale + shift_x),
            (int)(0.5 + points[i+1].y * scale + shift_y)
        );
    }

    return img;
}

void Fractal::saveImage(int width, int height) {
    Image img = getImage(width, height);
    img.erode();

    img.save("out.bmp");
    // img.save("pics/" + string + ".bmp");
}

