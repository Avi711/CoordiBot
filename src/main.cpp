#include "../include/robot.h"
#include <iostream>

int main(int argc, char *argv[]) {
    Robot bob;
    Vertex v(1,2,3);
    bob.navigateTo(v); // currently this will move to a mock location
    return 0;
}