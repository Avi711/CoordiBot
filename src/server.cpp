//
// Created by avi on 5/30/23.
//

#include "../include/server.h"

RestServer::RestServer(Robot* bob_) : listener_("http://localhost:8080") {
    listener_.support(methods::GET, std::bind(&RestServer::handleGet, this, std::placeholders::_1));
    listener_.support(methods::POST, std::bind(&RestServer::handlePost, this, std::placeholders::_1));
    this->bob = bob_;
}

void RestServer::start() {
    listener_.open().wait();
    std::cout << "Server started\n";
}

void RestServer::stop() {
    listener_.close().wait();
    std::cout << "Server stopped\n";
}

void RestServer::handleGet(http_request request) {
    if (request.request_uri().path() == "/isBusy") {
        json::value response;
        response["isBusy"] = bob->isBusy();
        request.reply(status_codes::OK, response);
    }
    else {
        request.reply(status_codes::NotFound);
    }
}

void RestServer::handlePost(http_request request) {
    if (request.request_uri().path() == "/makeMeeting") {
        request.extract_json().then([=](json::value body) {
            // Extract meeting details from the request body JSON

            std::cout<< body["title"].size() << std::endl;


            json::value response;
            request.reply(status_codes::OK, response);


            for (int i = 0; i < body["invited"].size(); ++i) {
                bob->navigateTo(body["invited"][i].as_integer());
            }

            //std::string meetingTitle = body["title"].as_string();
            //std::string meetingTime = body["time"].as_string();
            //std::cout << meetingTitle << std::endl;
            // Prepare the response JSON

            //response["title"] = json::value::string(meetingTitle);
            //response["time"] = json::value::string(meetingTime);


        });
    }
    else {
        request.reply(status_codes::NotFound);
    }
}
