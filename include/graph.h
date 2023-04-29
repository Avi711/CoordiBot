//
// Created by odin on 4/29/23.
//

#ifndef COORDIBOT_GRAPH_H
#define COORDIBOT_GRAPH_H

class Vertex{
private:
    double x;
    double y;
public:
    Vertex(double x=0,double y=0);
    double getX();
    double getY();
};

#endif //COORDIBOT_GRAPH_H
