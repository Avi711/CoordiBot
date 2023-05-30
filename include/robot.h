
#ifndef COORDIBOT_ROBOT_H
#define COORDIBOT_ROBOT_H

#include <libplayerc++/playerc++.h>
#include "graph.h"
#include <array>
#include "constants.h"
#include <map>
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
    std::map<int, Vertex*>* map;
    bool isBusy_ = false;
    void goTo(Vertex v);
    int navigateTo(Vertex v);
    Vertex* goToNearestPoint();
    void rotateToVertex(Vertex v);
    void setSpeed(double x, double y);
public:
    Robot();
    Position getPos();
    double getSonar(int index);
    int navigateTo(int id);
    bool isBusy();
    std::map<int, Vertex*>* getMap();
};

#endif //COORDIBOT_ROBOT_H
