#include "../include/robot.h"
#include "../include/utils.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <map>
#include <float.h>
#define AVOID_DISTANCE 0.35


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


Robot::Robot() : robot(PLAYER_IP, PLAYER_PORT), pos2d(&robot, 0), sonarProxy(&robot, 0) {
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
    this->robotMutex_.lock();
    robot.Read();
    this->robotMutex_.unlock();
    return sonarProxy[index];
}


int Robot::goTo(Vertex v) {
    cout << "Going to:::::::::::;" << v.getId() << endl;
    double vy = v.getY(), vx = v.getX();
    this->rotateToVertex(v);
    cout << "after rotate" << endl;
    double prev = DBL_MAX;
    double distance = 0;
    while (true) {
        Position pos = this->getPos();
        double ry = pos.getY(), rx = pos.getX();
        distance = calculateEuclideanDistance(rx,ry,vx,vy);
        //cout << "ddddddxxxxxxxx: " << std::abs(vx - rx) << "  ddddddyyyyyyy:  " << std::abs(vy - ry) << endl;
        if (distance > prev || (std::abs(vx - rx) < 0.21 && std::abs(vy - ry) < 0.21)) {
            //cout << "dx: " << std::abs(vx - rx) << "  dy:  " << std::abs(vy - ry) << endl;
            this->setSpeed(0, 0);
            return 0;
        }
        
        this->setSpeed(MAX_MOVEMENT_SPEED, 0);
        if (this->isObstacle()) {
                this->AvoidObstacles(v);
                return -1;
        }
        prev = distance;
    }
}

int Robot::navigateTo(int id) {
    if (isBusy_) {
        cout << "The robot is busy" << endl;
        return -1;
    }
    if (id < 0) {
        cout << "Not valid ID";
        return -1;
    }
    this->isBusy_ = true;
    Vertex v = *this->map->at(id);
    int res = this->navigateTo(v);
    this->isBusy_ = false;
    return res;
}

// return -1 if can't navigate (no target id) or there is an obstacle.
int Robot::navigateTo(Vertex v) {
    cout << "Navigatig to:::::::::::;" << v.getId() << endl;
    if (v.getId() == -1) {
        cout << "No vertex id to navigate to.";
        return -1;
    }
    int estimated_time = (abs(this->getPos().getX() - v.getX()) + abs(this->getPos().getY() + v.getY()) / (MAX_MOVEMENT_SPEED / 2)) * 5 + 40;
    auto start_time = std::chrono::high_resolution_clock::now();
    auto end_time = start_time + std::chrono::seconds(estimated_time);
    while(std::chrono::high_resolution_clock::now() < end_time) {
        int problem = 0;
        Vertex* cur = goToNearestPoint();
        if (cur != nullptr) {
            std::vector<Vertex> route = getRoute(*cur, v, this->map); // currently uses mock start and goal points
            for (auto it = route.begin() + 1; it != route.end(); ++it) {
                auto check = (it + 1);
                Position pos = this->getPos();
                double deg = getDegree({pos.getX(), pos.getY()}, {check->getX(), check->getY()});
                double deg_diff = getRadiansDistance(this->getPos().getDeg(), deg);
                if (deg_diff < 0.005)
                    continue;
                if (this->goTo(*it) == -1) {
                    problem = 1;
                    break;
                }
            }
            if (problem)
                continue;
            return 0;
        }
    }
    return -1;
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
        double epsilon = 0.05;
        if (pos.getDeg() > M_PI / 2 - epsilon && deg < -M_PI / 2 + epsilon)
            rotation_speed = abs(rotation_speed);
        else if (deg > M_PI / 2 - epsilon && pos.getDeg() < -M_PI / 2 + epsilon)
            rotation_speed = abs(rotation_speed) * -1;

        this->setSpeed(0, rotation_speed);
        pos = this->getPos();
    }
}

Position Robot::getPos() {
    this->robotMutex_.lock();
    robot.Read();
    this->robotMutex_.unlock();
    return {pos2d.GetXPos(), pos2d.GetYPos(), pos2d.GetYaw()};
}

Speed Robot::getSpeed() {
    robot.Read();
    return {pos2d.GetXSpeed(), pos2d.GetYSpeed(), pos2d.GetYawSpeed()};
}

std::map<int, Vertex *> *Robot::getMap() {
    return this->map;
}

Vertex *Robot::goToNearestPoint() {
    Position cur_pos = this->getPos();
    Vertex cur_vertex(cur_pos.getX(), cur_pos.getY());
    Vertex *min_vertex = getNearestStop(cur_vertex, *this->map);
    if (getDistance(cur_vertex, *min_vertex) > NEARSET_POINT_THRESHOLD) {
        if (this->goTo(*min_vertex) == -1)
            return nullptr;
    }
    return min_vertex;
}

bool Robot::isBusy() {
    return this->isBusy_;
}

void Robot::readThread() {
    while (true) {
        if (!isBusy_) {
            this->robotMutex_.lock();
            robot.Read();
            this->robotMutex_.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
    }
}

void Robot::outputVoiceMessage() {
    // -- Insert voice message output here --
    std::this_thread::sleep_for(std::chrono::seconds(30));
}


void Robot::AvoidObstacles(Vertex v) {
    //will turn away at 60 degrees/sec
    double avoidTurnSpeed = 1;
    std::cout << "front" << getSonar(0) << std::endl;
    std::cout << "left" << getSonar(1) << std::endl;
    std::cout << "right" << getSonar(2) << std::endl;
    for (int i = 0; i <= 4; i++) {
        cout << " front: " << this->getSonar(0) << " left: " << this->getSonar(1) << " right: " << this->getSonar(2)
             << endl;
        double left = this->getSonar(1);
        double right = this->getSonar(2);
        if (this->getSonar(1) < AVOID_DISTANCE && left < right) {
            std::cout << "left" << std::endl;
            //turn right
            this->setSpeed(-0.6, (-1) * avoidTurnSpeed);

        } else if (this->getSonar(2) < AVOID_DISTANCE && right < left) {
            std::cout << "right" << std::endl;
            //turn left
            this->setSpeed(-0.6, avoidTurnSpeed);

        } else if (this->getSonar(0) < AVOID_DISTANCE) {
            std::cout << "front" << std::endl;
            //back off a little bit
            this->setSpeed(-0.6, 0);
        } else { this->setSpeed(0.6, 0); }
        sleep(1);


    }
    std::cout << "d" << std::endl;
}

int Robot::isObstacle() {
    if ((this->getSonar(0) <= AVOID_DISTANCE) || (this->getSonar(1) <= AVOID_DISTANCE) ||
        (this->getSonar(2) <= AVOID_DISTANCE)) { return 1; }
    return 0;
}

