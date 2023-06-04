
#ifndef COORDIBOT_ROBOT_H
#define COORDIBOT_ROBOT_H

#include <array>
#include <map>
#include <libplayerc++/playerc++.h>
#include "constants.h"
#include "graph.h"

using namespace PlayerCc;

class Position {
    double x;
    double y;
    double deg;
public:
    Position(double x, double y, double deg) : x(x), y(y), deg(deg) {};

    double getX() const { return x; }

    double getY() const { return y; }

    double getDeg() const { return deg; }
};


class Robot {
    PlayerClient robot;
    Position2dProxy pos2d;
    RangerProxy sonarProxy;
    std::map<int, Vertex *> *map;
    bool isBusy_ = false;

    void goTo(Vertex);

    Vertex *goToNearestPoint();

    void rotateToVertex(Vertex);

    void setSpeed(double, double);

    int navigateTo(Vertex);

public:
    Robot();

    Position getPos();

    double getSonar(int);

    int navigateTo(int);

    bool isBusy();

    std::map<int, Vertex *> *getMap();
};

#endif //COORDIBOT_ROBOT_H
