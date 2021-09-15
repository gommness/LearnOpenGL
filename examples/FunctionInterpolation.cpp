#include <iostream>
#include <cmath>
#include <vector>
#include <array>
#include <functional>

using Point = std::array<float, 3>;
using ParametricFunction = std::function<Point(const Point &)>;

template <typename T>
T lerp(T a, T b, double t) {
    return a+t*(b-a);
}

void printLine(const std::vector<Point>& line) {
    for (int i=0; i<line.size(); i++) {
        std::cout << line[i][0] << ", " << line[i][1] << ", " << line[i][2] << std::endl;
    }
}

std::vector<Point> generateLine(const Point & start, const Point & end, const size_t n_iter) {
    std::vector<Point> mesh;

    float xStart = start[0];
    float yStart = start[1];
    float zStart = start[2];

    float xEnd = end[0];
    float yEnd = end[1];
    float zEnd = end[2];

    double incr = 1.0/(n_iter+1);
    double acc = 0;

    for (int i = 0; i <= n_iter; i++, acc+=incr) {
        float x = lerp(xStart, xEnd, acc);
        float y = lerp(yStart, yEnd, acc);
        float z = lerp(zStart, zEnd, acc);
        mesh.push_back(Point{x, y, z});
    }
    mesh.push_back(end);
    return mesh;
}

std::vector<Point> transformLine(const std::vector<Point> & line, const ParametricFunction & f) {
    std::vector<Point> transformedLine;
    for (unsigned int i=0; i<line.size(); i++) {
        transformedLine.push_back(f(line[i]));
    }
    return transformedLine;
}

Point simpleTranslation(const Point & p) {
    return Point{p[0], p[0]*p[0], p[2]};
}


int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "not enough arguments" << std::endl;
        return 1;
    }
    const size_t nPoints = std::atol(argv[1]);
    Point start{-1.0f, 0.0f, 0.0f};
    Point end{1.0f, 0.0f, 0.0f};
    std::vector<Point> line = generateLine(start, end, nPoints);
    //printLine(line);
    std::vector<Point> transformedLine = transformLine(line, simpleTranslation);
    printLine(transformedLine);
    return 0;
}
