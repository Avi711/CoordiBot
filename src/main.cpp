#include "../include/robot.h"
#include <iostream>

int main(int argc, char *argv[]) {
    Robot bob;
    bob.navigateTo({}); // currently this will move to a mock location
    return 0;
}