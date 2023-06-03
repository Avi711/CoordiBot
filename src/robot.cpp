#include "../include/robot.h"
#include "../include/utils.h"
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
    this->map = generateMap();
}

void Robot::setSpeed(double x, double y) {
    pos2d.SetSpeed(x, y);
}

double Robot::getSonar(int index) {
    robot.Read();
    return sonarProxy[index];
}

void Robot::goTo(Vertex v) {
    cout << "Going to:::::::::::;" << v.getId() << endl;
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

int Robot::navigateTo(int id) {
    if (this->isBusy_) {
        cout << "The robot is busy" << endl;
        return -1;
    }
    if (id < 0) {
        cout << "Not valid ID";
        return -1;
    }
    this->isBusy_ = true;
    Vertex v = *this->map->at(id);
    this->navigateTo(v);
    this->isBusy_ = false;
    return 0;
}

// return -1 if can't navigate (no target id)
int Robot::navigateTo(Vertex v) {
    cout << "Navigatig to:::::::::::;" << v.getId() << endl;
    if (v.getId() == -1) {
        cout << "No vertex id to navigate to.";
        return -1;
    }
    Vertex cur = *goToNearestPoint();
    std::vector<Vertex> route = getRoute(cur, v, this->map); // currently uses mock start and goal points
    for (auto it = route.begin() + 1; it != route.end(); ++it) {
        Position pos = this->getPos();
        double deg = getDegree({pos.getX(), pos.getY()}, {v.getX(), v.getY()});
        double deg_diff = getRadiansDistance(this->getPos().getDeg(), deg);
        if (deg_diff < 0.007 && (it + 1) != route.end())
            continue;
        this->goTo(*it);
    }
    return 0;
}

void Robot::rotateToVertex(Vertex v) {
    cout << "Rotating to:::::::::::;" << v.getId() << endl;
    Position pos = this->getPos();
    double deg = getDegree({pos.getX(), pos.getY()}, {v.getX(), v.getY()});
    double rotation_speed;
    while (true) {
        double deg_diff = getRadiansDistance(pos.getDeg(), deg);
        cout << "DEG DIF: " << deg_diff << " Current degree: " << pos.getDeg() << " Targer degree: " << deg << endl;
        if (deg_diff < 0.005) {
            this->setSpeed(0, 0);
            return;
        }
        rotation_speed = getRotationSpeed(deg_diff) * std::copysign(1.0, deg - pos.getDeg());
        if (pos.getDeg() > M_PI / 2 && deg < -M_PI / 2)
            rotation_speed = abs(rotation_speed);
        else if (deg > M_PI / 2 && pos.getDeg() < -M_PI / 2)
            rotation_speed = abs(rotation_speed) * -1;

        this->setSpeed(0, rotation_speed);
        pos = this->getPos();
    }
}

Position Robot::getPos() {
    robot.Read();
    return {pos2d.GetXPos(), pos2d.GetYPos(), pos2d.GetYaw()};
}

std::map<int, Vertex *> *Robot::getMap() {
    return this->map;
}

Vertex *Robot::goToNearestPoint() {
    Position cur_pos = this->getPos();
    Vertex cur_vertex(cur_pos.getX(), cur_pos.getY());
    Vertex *min_vertex = getNearestStop(cur_vertex, *this->map);
    if (getDistance(cur_vertex, *min_vertex) > 0.1) {
        this->goTo(*min_vertex);
    }
    return min_vertex;
}

bool Robot::isBusy() {
    return this->isBusy_;
}


