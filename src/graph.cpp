#include "../include/graph.h"

Vertex::Vertex(double x, double y) :x(x),y(y){}
double Vertex::getX() {return this->x;}
double Vertex::getY() {return this->y;}

// TODO implement this func for real
std::vector<Vertex> getRoute(Vertex start, Vertex goal){
    std::vector<Vertex> v = {{1.33,-5.7}, {1.3,3}};
    return v;
}

// TODO fix this func
double getDegree(Vertex start, Vertex end){
    double dx = end.getX() - start.getX();
    double dy = end.getY() - start.getY();
    return std::atan2(dy, dx);
}

