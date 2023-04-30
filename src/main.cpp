#include <iostream>
#include <libplayerc++/playerc++.h>
#include "../include/graph.h"
#include "../include/robot.h"

using namespace std;
using namespace PlayerCc;

int main(int argc, char *argv[]) {
    Robot bob;
    while (true) {
        // read from the proxies
        cout << bob.getYaw() << endl;
        if (bob.getSonar(0) < 1.5) {
            bob.setSpeed(0,-1*90);
        }
        else {
            bob.setSpeed(10,0);
        }
    }
    return 0;
}
