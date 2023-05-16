
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
    ~Vertex() {delete neighbors;}
    double getX();
    double getY();
    int getId() const;
    double getDistance(const Vertex& other);
    std::vector<int>* getNeighbors();
    void addNeighbor(int n);
};

class Node {
    Vertex v;
    int state;
    Node* parent;
    double path_cost;
    int depth;
public:
    Node(const Vertex& v, Node* p = nullptr);
};





std::vector<Vertex> getRoute(Vertex start, Vertex goal);
double getDegree(Vertex start, Vertex end);

#endif //COORDIBOT_GRAPH_H
