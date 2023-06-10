
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
class Speed {
    double xSpeed;
    double ySpeed;
    double yawSpeed;
public:
    Speed(double xSpeed, double ySpeed,double yawSpeed) : xSpeed(xSpeed), ySpeed(ySpeed), yawSpeed(yawSpeed) {};
    double getXSpeed() const { return xSpeed; }
    double getYSpeed() const { return ySpeed; }
    double getYawSpeed() const { return yawSpeed; }
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
    Speed getSpeed();
    Position getPos();
    RangerProxy getSonar();
    int navigateTo(int id);
    bool isBusy();
    void AvoidObstacles(double forwardSpeed, double turnSpeed,RangerProxy sp);
    std::map<int, Vertex*>* getMap();
};

#endif //COORDIBOT_ROBOT_H
