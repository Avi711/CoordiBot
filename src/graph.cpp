#include "../include/graph.h"
#include <iostream>
#include "queue"
#include "map"
#include "algorithm"
#include "float.h"

using namespace std;

Vertex::Vertex(double x, double y) : id(-1), x(x),y(y){ neighbors = new std::vector<int>(); }
Vertex::Vertex(int id, double x, double y) :id(id), x(x),y(y){ neighbors = new std::vector<int>();}
int Vertex::getId() const { return id; }
std::vector<int> *Vertex::getNeighbors() { return neighbors;}
void Vertex::addNeighbor(int n) { neighbors->push_back(n); }

// TODO implement this func for real with dijkstra or something else
std::vector<Vertex> getRoute(Vertex start, Vertex goal, std::map<int, Vertex*>* mp){
    std::vector<Vertex> route = {
                             {-8.70 ,-9.65},{-12.48,-9.70},{-12.48,-4.28},{-12.48,0.29},{-12.48,3.16},{-12.48,6.07},{-12.48,7.17}
                             ,{-12.48, 11.78},{-12.48, 14.77},{-12.48,15.99},{-12.48,18.90},{-6.45,19.38},{-3.58,19.38}
                             ,{-0.80,19.38},{2.24,19.38},{6.89,19.38},{11.46, 19.38},{11.94,14.83},{11.94,10.22},{11.94,7.18},{11.94, 6.05}
                             ,{11.94,4.26},{11.94, 3.22},{11.94, -1.57},{11.94,-2.70},{11.94, -5.57},{11.94, -8.57}
                             ,{11.94, -11.09},{-1.32,-9.65}};
    cout << "in the getRout: " << mp->at(1)->getId() << endl;
    route = A_STAR(start,goal,mp);
    return route;
}

Node::Node(const Vertex& v,const Node *p): v(v), parent(p), state(v.getId()) {
    if (parent != nullptr) {
        path_cost = parent->path_cost + getDistance(this->v, parent->v);
        depth = parent->depth + 1;
    }
    else {
        path_cost = 0;
        depth = 0;
    }
}

const Node *Node::getParent() { return parent; }

double Node::getPathCost() const { return path_cost; }

int Node::getDepth() const { return depth; }

Vertex Node::getVertex() const { return this->v; }

int Node::getState() const { return state; }

vector<Vertex> Node::getPath() {
    vector<Vertex> res;
    const Node* cur = this;
    while(cur) {
        res.push_back(cur->v);
        cur = cur->parent;
    }
    reverse(res.begin(), res.end());
    return res;
}

// To do
std::vector<Node *> Node::expand(std::map<int, Vertex*>* mp) const {
    std::vector<Node *> temp;
    vector<int>* v_list = mp->at(this->state)->getNeighbors();
    for (int id : *v_list) {
        Vertex new_v = *mp->at(id);
        temp.push_back(new Node(new_v, this));
    }
    return temp;
}

void Node::setPathCost(double cost) { this->path_cost = cost; }

double getDegree(Vertex start, Vertex end)  {
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

double getRadiansDistance(double rad1, double rad2) {
    const double PI = std::acos(-1);
    double diff = std::fmod(std::abs(rad1 - rad2), 2*PI);
    return (diff > PI) ? 2*PI - diff : diff;
}

double getDistance(const Vertex& v1, const Vertex& v2) {
    double d_x = v1.getX() - v2.getX();
    double d_y = v1.getY() - v2.getY();
    return std::sqrt(d_x * d_x + d_y * d_y);
}


std::vector<Vertex> A_STAR(Vertex start, Vertex goal, std::map<int, Vertex*>* mp) {
    auto compareNodes = [goal](const Node* lhs, const Node* rhs) {
        return lhs->getPathCost() + getDistance(lhs->getVertex(), goal) > rhs->getPathCost() + getDistance(rhs->getVertex(), goal); // Smallest path first
    };
    std::priority_queue<Node*, std::vector<Node*>, decltype(compareNodes)> frontier(compareNodes);

    frontier.push(new Node(start));
    map<int, bool> close_list;
    map<int, Node*> frontier_map;
    frontier_map[start.getId()] = frontier.top();
    while (!frontier.empty()) {
        Node *node = frontier.top();
        frontier.pop();
        if (node->getState() == goal.getId())
            return node->getPath();
        close_list[node->getState()] = true;
        vector<Node*> child_list = node->expand(mp);
        for (Node* child: child_list) {
            if (close_list[child->getState()] == false && frontier_map[child->getState()] == nullptr) {
                frontier.push(child);
                frontier_map[child->getState()] = child;
            }
            else if (frontier_map[child->getState()] != nullptr && child->getPathCost() < frontier_map[child->getState()]->getPathCost()) {
                frontier_map[child->getState()]->setPathCost(DBL_MAX);
                frontier.push(child);
                frontier_map[child->getState()] = child;
            }
        }
    }
    return {};
}


