#include "../include/graph.h"

Vertex::Vertex(double x, double y) :x(x),y(y){}
double Vertex::getX() {return this->x;}
double Vertex::getY() {return this->y;}

// TODO implement this func for real with dijkstra or something else
std::vector<Vertex> getRoute(Vertex start, Vertex goal){
    std::vector<Vertex> v = {{1.33,-5.7}, {1.3,3},{1.33,-5.7}, {-7.22, -5.70} };
    return v;
}

double getDegree(Vertex start, Vertex end){
    double dx = end.getX() - start.getX();
    double dy = end.getY() - start.getY();
    if (dy == 0 && dx < 0)
        return M_PI;
    if (dx == 0 && dy > 0)
        return M_PI/2;
    if (dx == 0 && dy < 0)
        return -M_PI/2;
    return std::atan2(dy, dx);
}

