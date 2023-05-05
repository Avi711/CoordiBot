
#ifndef COORDIBOT_GRAPH_H
#define COORDIBOT_GRAPH_H
#include <vector>
#include <cmath>

class Vertex {
private:
    double x;
    double y;
public:
    Vertex(double x = 0.0, double y = 0.0);

    double getX();

    double getY();
};

std::vector<Vertex> getRoute(Vertex start, Vertex goal);

double getDegree(Vertex start, Vertex end);

#endif //COORDIBOT_GRAPH_H
