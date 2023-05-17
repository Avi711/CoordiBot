#include "../include/robot.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <map>


using namespace std;

double getRotationSpeed(double deg_diff) {
    if (deg_diff > ANGLE_PRECISION_1) {
        return ROTATION_SPEED_1;
    } else if (deg_diff > ANGLE_PRECISION_2) {
        return ROTATION_SPEED_2;
    } else if (deg_diff > ANGLE_PRECISION_3) {
        return ROTATION_SPEED_3;
    } else {
        return ROTATION_SPEED_4;
    }
}


Robot::Robot() : robot("localhost", 6665), pos2d(&robot, 0), sonarProxy(&robot, 0) {
    robot.Read();
    // enable motors
    pos2d.SetMotorEnable(1);
    // request geometries
    pos2d.RequestGeom();
    sonarProxy.RequestGeom();
    pos2d.SetSpeed(10, 0);
}

void Robot::setSpeed(double x, double y) {
    pos2d.SetSpeed(x, y);
}

double Robot::getSonar(int index) {
    robot.Read();
    return sonarProxy[index];
}

void Robot::goTo(Vertex v) {
    double vy = v.getY(), vx = v.getX();
    this->rotateToVertex(v);
    cout << "after rotate" << endl;
    while (true) {
        Position pos = this->getPos();
        double ry = pos.getY(), rx = pos.getX();
        if (std::abs(vx - rx) < 0.21 and std::abs(vy - ry) < 0.21) {
            this->setSpeed(0, 0);
            return;
        }
        this->setSpeed(1, 0);
    }
}

void Robot::navigateTo(Vertex v) {
    std::vector<Vertex> route = getRoute({}, {}); // currently uses mock start and goal points
    for (Vertex vertex: route) {
        this->goTo(vertex);
    }
}

void Robot::rotateToVertex(Vertex v) {
    Position pos = this->getPos();
    double deg = getDegree({pos.getX(), pos.getY()}, {v.getX(), v.getY()});
    cout << "degree: " << deg << endl;
    double rotation_speed;
    while (true) {
        double deg_diff = abs(pos.getDeg() - deg);
        cout << deg_diff << endl;
        if (deg_diff < 0.005) {
            this->setSpeed(0, 0);
            return;
        }
        rotation_speed = getRotationSpeed(deg_diff) * std::copysign(1.0, deg - pos.getDeg());
        if (deg > 0 && pos.getDeg() < 0)
            rotation_speed *= -1;
        this->setSpeed(0, rotation_speed);
        pos = this->getPos();
        cout << "rotation speed: " << rotation_speed << " position: " << pos.getDeg() << endl;
    }
}

Position Robot::getPos() {
    robot.Read();
    return {pos2d.GetXPos(),pos2d.GetYPos(), pos2d.GetYaw()};
}
