#include "../include/graph.h"

Vertex::Vertex(double x, double y) :x(x),y(y){}
double Vertex::getX() {return this->x;}
double Vertex::getY() {return this->y;}

// TODO implement this func for real with dijkstra or something else
std::vector<Vertex> getRoute(Vertex start, Vertex goal){
    std::vector<Vertex> v = {
                             {-8.70 ,-9.65},{-12.48,-9.70},{-12.48,-4.28},{-12.48,0.29},{-12.48,3.16},{-12.48,6.07},{-12.48,7.17}
                             ,{-12.48, 11.78},{-12.48, 14.77},{-12.48,15.99},{-12.48,18.90},{-6.45,19.38},{-3.58,19.38}
                             ,{-0.80,19.38},{2.24,19.38},{6.89,19.38},{11.46, 19.38},{11.94,14.83},{11.94,10.22},{11.94,7.18},{11.94, 6.05}
                             ,{11.94,4.26},{11.94, 3.22},{11.94, -1.57},{11.94,-2.70},{11.94, -5.57},{11.94, -8.57}
                             ,{11.94, -11.09},{-1.32,-9.65}};
    return v;
}

double getDegree(Vertex start, Vertex end){
    double dx = end.getX() - start.getX();
    double dy = end.getY() - start.getY();
    if (dy == 0 && dx < 0)
        return M_PI;
    if (dx == 0 && dy > 0)
        return M_PI/2;
    if (dx == 0 && dy < 0)
        return -M_PI/2;
    return std::atan2(dy, dx);
}

