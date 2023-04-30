//
// Created by avi on 4/30/23.
//
#include <libplayerc++/playerc++.h>
using namespace PlayerCc;

#ifndef COORDIBOT_ROBOT_H
#define COORDIBOT_ROBOT_H


class Robot {
    PlayerClient robot;
    Position2dProxy pos2d;
    RangerProxy sonarProxy;
public:
    Robot();
    double getXPos();
    double getYPos();
    double getYaw();
    void setSpeed(double x, double y);
    double getSonar(int index);
};


#endif //COORDIBOT_ROBOT_H