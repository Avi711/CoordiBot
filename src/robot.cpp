#include "../include/robot.h"
#include <iostream>
#include <chrono>
#include <thread>

#define deg90 1.57079633

Robot::Robot() : robot("localhost", 6665), pos2d(&robot, 0), sonarProxy(&robot, 0) {
    robot.Read();
    // enable motors
    pos2d.SetMotorEnable(1);
    // request geometries
    pos2d.RequestGeom();
    sonarProxy.RequestGeom();
    pos2d.SetSpeed(10,0);
}

void Robot::setSpeed(double x, double y) {
    pos2d.SetSpeed(x,y);
}

double Robot::getSonar(int index) {
    robot.Read();
    return sonarProxy[index];
}

void Robot::goTo(Vertex v) {
    double vy = v.getY(), vx = v.getX();
    this->rotateToVertex(v);
    while (true){
        std::array<double, 3> pos = this->getPos();
        double ry = pos[1], rx = pos[0];
        if(std::abs(vx - rx) < 0.21 and std::abs(vy - ry) < 0.21){
            this->setSpeed(0, 0);
            return;
        }
        this->setSpeed(1, 0);
    }
}

void Robot::navigateTo(Vertex v) {
    std::vector<Vertex> route = getRoute({},{}); // currently uses mock start and goal points
    for(Vertex vertex:route){
        this->goTo(vertex);
    }
}

// TODO fix this function (the farther you there faster you turn, for more information ask avi/leonardo)
void Robot::rotateToVertex(Vertex v) {
    std::array<double, 3> pos = this->getPos();
    double deg = getDegree({pos[0],pos[1]},{v.getX(),v.getY()});
    while (true) {
        std::cout<<pos[2]<<std::endl;
        if(std::abs(pos[2]-deg)<0.001){
            this->setSpeed(0, 0);
            return;
        }
        if(deg>0){
            this->setSpeed(0, 0.25);
        } else{
            this->setSpeed(0, -0.25);
        }
        pos = this->getPos();
    }
}

std::array<double, 3> Robot::getPos() {
    robot.Read();
    return std::array<double, 3>{pos2d.GetXPos(),pos2d.GetYPos(),pos2d.GetYaw()};
}
