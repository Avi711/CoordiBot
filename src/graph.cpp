#include "../include/graph.h"

Vertex::Vertex(double x, double y) :x(x),y(y){}
double Vertex::getX() {return this->x;}
double Vertex::getY() {return this->y;}

// TODO implement this func for real with dijkstra or something else
std::vector<Vertex> getRoute(Vertex start, Vertex goal){
    std::vector<Vertex> v = {{1.33,-5.7}, {1.3,3},{1.33,-5.7}, {-7.22, -5.70},
                             {-8.70 -9.65},{-12.48,-9.70},{-12.48,-4.28},{-12.48,0.29},{-12.48,3.16},{-12.48,6.07},{-12.48,7.17}
                             ,{-12.48 11.78},{-12.48 14.77},{-12.48,15.99},{-12.48,18.90},{-6.45,19.57},{-3.58,19.57}
                             ,{-0.80,19.57},{2.24,19.57},{6.89,19.57},{11.46 19.57},{12.20,14.83},{12.20,10.22},{12.20,7.18},{12.20 6.05}
                             ,{12.20, 3.22},{11.46,4.26},{11.46,3.26},{12.20, -1.57},{12.20 -2.70},{12.20 -5.57},{12.20 -8.57}
                             ,{12.20 -11.09},{-1.32,-10.09}};


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

