
#ifndef COORDIBOT_GRAPH_H
#define COORDIBOT_GRAPH_H
#include <vector>
#include <cmath>
#include <map>

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
    const Node* parent;
    int depth;
public:
    Node(const Vertex& v,const Node* p = nullptr);
    const Node* getParent();
    Vertex getVertex() const;
    double getPathCost() const;
    void setPathCost(double cost);
    int getState() const;
    int getDepth() const;
    std::vector<Vertex> getPath();
    std::vector<Node*> expand(std::map<int, Vertex*>* mp) const;
    double path_cost;//TODO switch to private

};

std::vector<Vertex> getRoute(Vertex start, Vertex goal);
double getDegree(Vertex start, Vertex end);
double getDistance(const Vertex& v1, const Vertex& v2);

std::vector<Vertex> A_STAR(Vertex start, Vertex goal, std::map<int, Vertex*>* mp);



#endif //COORDIBOT_GRAPH_H
