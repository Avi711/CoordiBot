#ifndef COORDIBOT_SERVER_H
#define COORDIBOT_SERVER_H

#include <cpprest/http_listener.h>
#include "../include/robot.h"
#include "../include/graph.h"
#include <thread>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;


class RestServer {
    http_listener listener_;
    Robot *bob;
    std::vector<Vertex> cachedPlan;

    void handleGet(http_request request);

    void handlePost(http_request request);

    void handleGetStatus(http_request request);

    void handleGetMakeMeeting(http_request request);

    void handlePostMakeMeeting(http_request request);

public:
    RestServer(Robot *bob);

    void start();

    void stop();
};


#endif //COORDIBOT_SERVER_H
