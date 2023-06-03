#include <iostream>
#include "../include/robot.h"
#include "../include/server.h"

using namespace std;

int main() {
    auto *bob = new Robot;
    RestServer server(bob);
    server.start();
    std::cout << "Press Enter to exit.\n";
    std::cin.ignore();
    server.stop();
    return 0;
}
