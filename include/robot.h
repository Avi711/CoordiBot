
#ifndef COORDIBOT_ROBOT_H
#define COORDIBOT_ROBOT_H

#include <libplayerc++/playerc++.h>
#include "graph.h"
#include <array>
#include "constants.h"
using namespace PlayerCc;

class Robot {
    PlayerClient robot;
    Position2dProxy pos2d;
    RangerProxy sonarProxy;
public:
    Robot();
    std::array<double,3> getPos();
    double getSonar(int index);
    void setSpeed(double x, double y);
    void rotateToVertex(Vertex v);
    void goTo(Vertex v);
    void navigateTo(Vertex v);

};



#endif //COORDIBOT_ROBOT_H
