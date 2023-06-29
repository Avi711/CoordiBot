#ifndef COORDIBOT_UTILS_H
#define COORDIBOT_UTILS_H

#include "../include/graph.h"
#include <map>

double calculateEuclideanDistance(double x1, double y1, double x2, double y2);
std::map<int, Vertex *> *generateMap();

#endif //COORDIBOT_UTILS_H
