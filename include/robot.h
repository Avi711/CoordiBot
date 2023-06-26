
#ifndef COORDIBOT_ROBOT_H
#define COORDIBOT_ROBOT_H

#include <array>
#include <map>
#include <libplayerc++/playerc++.h>
#include <mutex>
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

class Speed {
    double xSpeed;
    double ySpeed;
    double yawSpeed;
public:
    Speed(double xSpeed, double ySpeed, double yawSpeed) : xSpeed(xSpeed), ySpeed(ySpeed), yawSpeed(yawSpeed) {};

    double getXSpeed() const { return xSpeed; }

    double getYSpeed() const { return ySpeed; }

    double getYawSpeed() const { return yawSpeed; }
};

class Robot {
    PlayerClient robot;
    Position2dProxy pos2d;
    RangerProxy sonarProxy;
    std::map<int, Vertex *> *map;
    bool isBusy_ = false;
    std::mutex robotMutex_;

    void goTo(Vertex);

    Vertex *goToNearestPoint();

    void rotateToVertex(Vertex);

    void setSpeed(double, double);

    int navigateTo(Vertex);


public:
    Robot();

    Speed getSpeed();

    Position getPos();

    double getSonar(int);

    int navigateTo(int);


    bool isBusy();

    std::map<int, Vertex *> *getMap();

    void readThread();

    void outputVoiceMessage();

    void AvoidObstacles(Vertex v);
};

#endif //COORDIBOT_ROBOT_H
