//
// Created by avi on 4/30/23.
//

#include "../include/robot.h"


Robot::Robot() : robot("localhost", 6665), pos2d(&robot, 0), sonarProxy(&robot, 0) {
    robot.Read();
    // enable motors
    pos2d.SetMotorEnable(1);
    // request geometries
    pos2d.RequestGeom();
    sonarProxy.RequestGeom();
    pos2d.SetSpeed(10,0);
}

double Robot::getXPos() {
    robot.Read();
    return pos2d.GetXPos();
}

double Robot::getYPos() {
    robot.Read();
    return pos2d.GetYPos();
}

double Robot::getYaw() {
    robot.Read();
    return pos2d.GetYaw();
}

void Robot::setSpeed(double x, double y) {
    pos2d.SetSpeed(x,y);
}

double Robot::getSonar(int index) {
    robot.Read();
    return sonarProxy[index];
}
