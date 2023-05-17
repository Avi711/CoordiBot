
#ifndef COORDIBOT_ROBOT_H
#define COORDIBOT_ROBOT_H

#include <libplayerc++/playerc++.h>
#include "graph.h"
#include <array>
#include "constants.h"
using namespace PlayerCc;

class Position {
    double x;
    double y;
    double deg;
public:
    Position(double x, double y,double deg) : x(x), y(y), deg(deg) {};
    double getX() const { return x; }
    double getY() const { return y; }
    double getDeg() const { return deg; }
};


class Robot {
    PlayerClient robot;
    Position2dProxy pos2d;
    RangerProxy sonarProxy;
public:
    Robot();
    Position getPos();
    double getSonar(int index);
    void setSpeed(double x, double y);
    void rotateToVertex(Vertex v);
    void goTo(Vertex v);
    void navigateTo(Vertex v);
};

#endif //COORDIBOT_ROBOT_H
