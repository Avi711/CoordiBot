#include "../include/robot.h"
#include "../include/graph.h"
#include "../include/utils.h"
#include <iostream>
#include <queue>
#include "map"
#include "../include/graph.h"

using namespace std;

int main(int argc, char *argv[]) {

    std::map<int, Vertex*>* mp = generateMap();


    Vertex start(1,-8.7,-9.65);
    Vertex goal(27,-12.48,15.99);
    std::vector<Vertex> path = A_STAR(start, goal,mp);


    for (Vertex v : path) {
        cout << v.getId() << endl;
    }


    return 0;

    Robot bob;
    Vertex v(1,2,3);
    bob.navigateTo({}); // currently this will move to a mock location

    Vertex target(1,1,2);
    auto compareNodes = [target](const Node* lhs, const Node* rhs) {
        return lhs->getPathCost() + getDistance(lhs->getVertex(), target) > rhs->getPathCost() + getDistance(rhs->getVertex(), target); // Smallest path first
    };
    std::priority_queue<Node*, std::vector<Node*>, decltype(compareNodes)> nodeQueue(compareNodes);
    Vertex v1(1,1,2);
    Node* n1 = new Node(v1);
    Vertex v2(2,1,2);
    Node* n2 = new Node(v2,n1);
    Vertex v3(3,1,2);
    Node* n3 = new Node(v3,n2);

    vector<Vertex> vec = n3->getPath();

    for (Vertex v : vec) {
        cout << "id: " << v.getId() << endl;
    }

    n1->path_cost = 5;
    n2->path_cost = 4;
    n3->path_cost = 3;
    nodeQueue.push(n3);
    nodeQueue.push(n1);
    nodeQueue.push(n2);
    cout << nodeQueue.top()->path_cost << endl;

    return 0;
}