
#ifndef COORDIBOT_GRAPH_H
#define COORDIBOT_GRAPH_H

#include <vector>
#include <cmath>
#include <map>

class Vertex {
    int id;
    double x;
    double y;
    std::vector<int> *neighbors;
public:
    Vertex(double = 0.0, double = 0.0);

    Vertex(int, double = 0.0, double = 0.0);

    //~Vertex() {delete neighbors;}
    double getX() const { return this->x; }

    double getY() const { return this->y; }

    int getId() const;

    bool operator<(const Vertex &other) const {
        return id < other.id;
    }

    std::vector<int> *getNeighbors();

    void addNeighbor(int);
};

class Node {
    Vertex v;
    int state;
    const Node *parent;
    int depth;
    double path_cost;
public:
    Node(const Vertex &, const Node * = nullptr);

    const Node *getParent();

    Vertex getVertex() const;

    double getPathCost() const;

    void setPathCost(double);

    int getState() const;

    int getDepth() const;

    std::vector<Vertex> getPath();

    std::vector<Node *> expand(std::map<int, Vertex *> *) const;
};

std::vector<Vertex> getRoute(Vertex, Vertex, std::map<int, Vertex *> *);

double getDegree(Vertex, Vertex);

double getDistance(const Vertex &, const Vertex &);

double getRadiansDistance(double, double);

std::tuple<double, std::vector<Vertex>> A_STAR(Vertex, Vertex, std::map<int, Vertex *> *);

std::tuple<std::vector<Vertex>, double>
getBestPlan(Vertex, std::vector<int> &, std::map<int, Vertex *> *);

Vertex *getNearestStop(const Vertex &, const std::map<int, Vertex *> &);

#endif //COORDIBOT_GRAPH_H
