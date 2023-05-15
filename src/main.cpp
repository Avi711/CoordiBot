#include <iostream>
#include <libplayerc++/playerc++.h>
#include "../include/graph.h"

using namespace std;
using namespace PlayerCc;


int main(int argc, char *argv[]) {

    PlayerClient robot("localhost", 6665);
    Position2dProxy  pos2d(&robot,0);
    RangerProxy      sonarProxy(&robot,0);

    // enable motors
    pos2d.SetMotorEnable(1);

    // geometries request
    pos2d.RequestGeom();
    sonarProxy.RequestGeom();
    pos2d.SetSpeed(10,0);

    Vertex* x = new Vertex(0,0);

    while (true) {
        // read from the proxies
        robot.Read();
        cout << sonarProxy[0] << endl;
        if (sonarProxy[0] < 1.5) {
            pos2d.SetSpeed(0,-1 * 90);
        }
        else {
            pos2d.SetSpeed(10,0);
        }


    }
    return 0;
}
