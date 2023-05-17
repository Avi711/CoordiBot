
#ifndef COORDIBOT_GRAPH_H
#define COORDIBOT_GRAPH_H
#include <vector>
#include <cmath>

class Vertex {
    int id;
    double x;
    double y;
    std::vector<int>*neighbors;
public:
    Vertex(double x = 0.0, double y = 0.0);
    Vertex(int id, double x = 0.0, double y = 0.0);
    //~Vertex() {delete neighbors;}
    double getX() const {return this->x;}
    double getY() const {return this->y;}
    int getId() const;
    std::vector<int>* getNeighbors();
    void addNeighbor(int n);
};

class Node {
    Vertex v;
    int state;
    Node* parent;
    int depth;
public:
    double path_cost;
    Node(const Vertex& v, Node* p = nullptr);
    Node* getParent();
    double getPathCost() const;
    int getDepth() const;
};

std::vector<Vertex> getRoute(Vertex start, Vertex goal);
double getDegree(Vertex start, Vertex end);
double getDistance(const Vertex& v1, const Vertex& v2);
#endif //COORDIBOT_GRAPH_H
