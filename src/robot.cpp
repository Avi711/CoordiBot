#include "../include/robot.h"
#include "../include/utils.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <map>
#define AVOID_DISTANCE 0.1


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
    this->robotMutex_.lock();
    robot.Read();
    this->robotMutex_.unlock();
    return sonarProxy[index];
}

void Robot::goTo(Vertex v) {
    while(true) {
        cout << "Going to:::::::::::;" << v.getId() << endl;
        double vy = v.getY(), vx = v.getX();
        this->rotateToVertex(v);
        cout << "after rotate" << endl;
        std::cout << "goto" << std::endl;

        while (true) {
            Position pos = this->getPos();
            double ry = pos.getY(), rx = pos.getX();
            if (std::abs(vx - rx) < 0.21 and std::abs(vy - ry) < 0.21) {
                this->setSpeed(0, 0);
                return;
            }
            this->setSpeed(MAX_MOVEMENT_SPEED, 0);
            // check for existence obstacle
            if ((this->getSonar(0) <= AVOID_DISTANCE) || (this->getSonar(1) <= AVOID_DISTANCE) ||
                (this->getSonar(2) <= AVOID_DISTANCE)) { return this->AvoidObstacles(v); break; }
        }
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
        this->goTo(*min_vertex);
    }
    return min_vertex;
}

bool Robot::isBusy() {
    return this->isBusy_;
}

void Robot::readThread() {
    while (true) {
        this->robotMutex_.lock();
        robot.Read();
        this->robotMutex_.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}

void Robot::outputVoiceMessage() {
    // -- Insert voice message output here --
    std::this_thread::sleep_for(std::chrono::seconds(30));
}


void  Robot::AvoidObstacles(Vertex v)
{
    //will turn away at 60 degrees/sec
    int avoidTurnSpeed = 10;
    std::cout<<"front"<<getSonar(0)<<std::endl;
    std::cout<<"left"<<getSonar(1)<<std::endl;
    std::cout<<"right"<<getSonar(2)<<std::endl;
    for (int i=0 ; i<=4;i++) {
        if (this->getSonar(1) < AVOID_DISTANCE) {
            std::cout << "left" << std::endl;
            //turn right
            this->setSpeed(-0.2, (-1) * avoidTurnSpeed);

        } else if (this->getSonar(2) < AVOID_DISTANCE) {
            std::cout << "right" << std::endl;
            //turn left
            this->setSpeed(-0.2, avoidTurnSpeed);

        } else if (this->getSonar(0) < AVOID_DISTANCE) {
            std::cout << "front" << std::endl;
            //back off a little bit
            this->setSpeed(-0.2, avoidTurnSpeed);
        }
        else{this->setSpeed(0.6,0);}
        sleep(1);


    }
    this->navigateTo(v);
    std::cout<<"d"<<std::endl;
//    return this->goTo(v);
}

