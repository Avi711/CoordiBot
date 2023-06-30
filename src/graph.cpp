#include "../include/graph.h"
#include <float.h>
#include <iostream>
#include <climits>
#include <queue>
#include <map>
#include <algorithm>

using namespace std;

Vertex::Vertex(double x, double y) : id(-1), x(x), y(y) { neighbors = new std::vector<int>(); }

Vertex::Vertex(int id, double x, double y) : id(id), x(x), y(y) { neighbors = new std::vector<int>(); }

int Vertex::getId() const { return id; }

std::vector<int> *Vertex::getNeighbors() { return neighbors; }

void Vertex::addNeighbor(int n) { neighbors->push_back(n); }

std::vector<Vertex> getRoute(Vertex start, Vertex goal, std::map<int, Vertex *> *mp) {
    auto route = A_STAR(start, goal, mp);
    return std::get<1>(route);
}

Node::Node(const Vertex &v, const Node *p) : v(v), parent(p), state(v.getId()) {
    if (parent != nullptr) {
        path_cost = parent->path_cost + getDistance(this->v, parent->v);
        depth = parent->depth + 1;
    } else {
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
    const Node *cur = this;
    while (cur) {
        res.push_back(cur->v);
        cur = cur->parent;
    }
    reverse(res.begin(), res.end());
    return res;
}

// To do
std::vector<Node *> Node::expand(std::map<int, Vertex *> *mp) const {
    std::vector<Node *> temp;
    vector<int> *v_list = mp->at(this->state)->getNeighbors();
    for (int id: *v_list) {
        Vertex new_v = *mp->at(id);
        temp.push_back(new Node(new_v, this));
    }
    return temp;
}

void Node::setPathCost(double cost) { this->path_cost = cost; }

double getDegree(Vertex start, Vertex end) {
    double dx = end.getX() - start.getX();
    double dy = end.getY() - start.getY();
    if (dy == 0 && dx < 0)
        return M_PI;
    if (dx == 0 && dy > 0)
        return M_PI / 2;
    if (dx == 0 && dy < 0)
        return -M_PI / 2;
    return std::atan2(dy, dx);
}

double getRadiansDistance(double rad1, double rad2) {
    const double PI = std::acos(-1);
    double diff = std::fmod(std::abs(rad1 - rad2), 2 * PI);
    return (diff > PI) ? 2 * PI - diff : diff;
}

double getDistance(const Vertex &v1, const Vertex &v2) {
    double d_x = v1.getX() - v2.getX();
    double d_y = v1.getY() - v2.getY();
    return std::sqrt(d_x * d_x + d_y * d_y);
}


std::tuple<double, std::vector<Vertex>> A_STAR(Vertex start, Vertex goal, std::map<int, Vertex *> *mp) {
    auto compareNodes = [goal](const Node *lhs, const Node *rhs) {
        return lhs->getPathCost() + getDistance(lhs->getVertex(), goal) >
               rhs->getPathCost() + getDistance(rhs->getVertex(), goal); // Smallest path first
    };
    std::priority_queue<Node *, std::vector<Node *>, decltype(compareNodes)> frontier(compareNodes);

    frontier.push(new Node(start));
    map<int, bool> close_list;
    map<int, Node *> frontier_map;
    frontier_map[start.getId()] = frontier.top();
    while (!frontier.empty()) {
        Node *node = frontier.top();
        frontier.pop();
        if (node->getState() == goal.getId())
            return std::make_tuple(node->getPathCost(), node->getPath());
        close_list[node->getState()] = true;
        vector<Node *> child_list = node->expand(mp);
        for (Node *child: child_list) {
            if (!close_list[child->getState()] && frontier_map[child->getState()] == nullptr) {
                frontier.push(child);
                frontier_map[child->getState()] = child;
            } else if (frontier_map[child->getState()] != nullptr &&
                       child->getPathCost() < frontier_map[child->getState()]->getPathCost()) {
                frontier_map[child->getState()]->setPathCost(DBL_MAX);
                frontier.push(child);
                frontier_map[child->getState()] = child;
            }
        }
    }
    return {};
}


std::tuple<std::vector<Vertex>, double>
getBestPlan(Vertex current, std::vector<int> &destinations, std::map<int, Vertex *> *mp) {
    std::vector<Vertex> currPlan;
    for (auto id: destinations) {
        try {
            currPlan.push_back(*mp->at(id));
        } catch (std::exception& e) {
            return std::make_tuple(std::vector<Vertex>(), -1);
        }
    }
    std::vector<Vertex> bestPlan = {current};
    Vertex lastStop = current;
    double planDist = 0;
    while (!currPlan.empty()) {
        double min_dist = INT_MAX;
        for (auto it: currPlan) {
            auto route = A_STAR(current, it, mp);
            double cost = std::get<0>(route);
            if (cost < min_dist) {
                min_dist = cost;
                lastStop = it;
            }
        }
        planDist += min_dist;
        auto it = std::find_if(currPlan.begin(), currPlan.end(),
                               [lastStop](const Vertex &obj) {
                                   return obj.getId() == lastStop.getId();
                               });

        currPlan.erase(it);
        bestPlan.push_back(lastStop);
        current = lastStop;
    }
    return std::make_tuple(std::move(bestPlan), planDist);
}

Vertex *getNearestStop(const Vertex &current, const map<int, Vertex *> &map) {
    double min_dist = INT_MAX;
    Vertex *min_vertex;
    for (auto p: map) {
        double dist = getDistance(current, *p.second);
        if (dist < min_dist) {
            min_vertex = p.second;
            min_dist = dist;
        }
    }
    return min_vertex;
}