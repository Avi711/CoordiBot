#include "../include/robot.h"
#include "../include/graph.h"
#include <iostream>
#include <queue>

using namespace std;

int main(int argc, char *argv[]) {
    Robot bob;
    Vertex v(1,2,3);
    bob.navigateTo({}); // currently this will move to a mock location

//    Vertex target(1,1,2);
//    auto compareNodes = [target](const Node* lhs, const Node* rhs) {
//        return lhs->getPathCost() +  > rhs->getPathCost(); // Smallest path first
//    };
//    std::priority_queue<Node*, std::vector<Node*>, decltype(compareNodes)> nodeQueue(compareNodes);
//    Vertex v1(1,1,2);
//    Node* n1 = new Node(v1);
//    Node* n2 = new Node(v1);
//    Node* n3 = new Node(v1);
//    n1->path_cost = 5;
//    n2->path_cost = 4;
//    n3->path_cost = 3;
//
//
//    nodeQueue.push(n3);
//    nodeQueue.push(n1);
//    nodeQueue.push(n2);
//    cout << nodeQueue.top()->path_cost << endl;

    return 0;
}