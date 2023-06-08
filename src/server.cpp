#include "../include/server.h"

RestServer::RestServer(Robot *bob_) : listener_("http://localhost:8080") {
    listener_.support(methods::GET, std::bind(&RestServer::handleGet, this, std::placeholders::_1));
    listener_.support(methods::POST, std::bind(&RestServer::handlePost, this, std::placeholders::_1));
    bob = bob_;
}

void RestServer::start() {
    listener_.open().wait();
    std::cout << "Server started\n";
}

void RestServer::stop() {
    listener_.close().wait();
    std::cout << "Server stopped\n";
}

void handleNotFound(http_request request) {
    request.reply(status_codes::NotFound);
}

std::map<std::string, std::string> getParamsFromGetRequest(std::map<std::string, std::string> queryItems) {
    std::map<std::string, std::string> parameters;
    for (const auto &item: queryItems) {
        std::string decodedKey = web::uri::decode(item.first);
        std::string decodedVal = web::uri::decode(item.second);
        std::string finalKey = decodedKey.substr(1, decodedKey.length() - 2);
        std::string finalVal = decodedVal.substr(1, decodedVal.length() - 2);
        parameters.insert(std::make_pair(finalKey, finalVal));
    }
    return std::move(parameters);
}

void RestServer::handleGetStatus(http_request request) {
    json::value response;
    response[DATA_PARAM] = json::value::object(
            {{STATUS_PARAM, json::value::string(bob->isBusy() ? BUSY : AVAILABLE)}});
    request.reply(status_codes::OK, response);
}

void RestServer::handleGetMakeMeeting(http_request request) {
    json::value response;
    if (bob->isBusy()) {
        response[DATA_PARAM] = json::value::object(
                {{MSG_PARAM, json::value::string(ROBOT_BUSY_ERROR_MSG)}});
        request.reply(status_codes::Conflict, response);
        return;
    }

    auto query = request.request_uri().query();
    std::map<std::string, std::string> parameters = getParamsFromGetRequest(web::uri::split_query(query));
    if (parameters.find(INVITED_PARAM) == parameters.end()) {
        response[DATA_PARAM] = json::value::object(
                {{MSG_PARAM, json::value::string(INVITED_ERROR_MSG)}});
        request.reply(status_codes::BadRequest, response);
        return;
    }

    std::stringstream ss(parameters[INVITED_PARAM]);
    std::string token;
    std::vector<int> destinations;
    while (std::getline(ss, token, ',')) {
        destinations.push_back(std::stoi(token));
    }

    Position cur_pos = bob->getPos();
    Vertex cur_vertex(cur_pos.getX(), cur_pos.getY());
    auto mp = bob->getMap();
    auto start = *getNearestStop(cur_vertex, *mp);
    auto route = getBestPlan(start, destinations, mp);

    if (std::get<1>(route) < 0) {
        response[DATA_PARAM] = json::value::object(
                {{MSG_PARAM, json::value::string(INVALID_IDS_MSG)}});
        request.reply(status_codes::BadRequest, response);
        return;
    }
    // TODO add validation of requester
    cachedPlan = std::get<0>(route);
    // TODO return time and not meters

    double planCost = std::get<1>(route) + getDistance(cur_vertex, start);
    response[DATA_PARAM] = json::value::object(
            {{ESTIMATED_TIME_PARAM, planCost}});
    request.reply(status_codes::OK, response);
    std::cout << "finished get\n";
}

void RestServer::handleGet(http_request request) {
    auto path = request.request_uri().path();
//    std::unordered_map<std::string, void (RestServer::*)(
//            http_request)> pathMap = {{STATUS_PATH,       &RestServer::handleGetStatus},
//                                      {MAKE_MEETING_PATH, &RestServer::handleGetMakeMeeting}};
//    auto it = pathMap.find(path);
//    if (it != pathMap.end()) {
//        (this->*(it->second))(request);
//    } else {
//        handleNotFound(request);
//    }

    if (path == STATUS_PATH) {
        handleGetStatus(request);
    } else if (path == MAKE_MEETING_PATH) {
        handleGetMakeMeeting(request);
    } else {
        handleNotFound(request);
    }
    return;
}

void RestServer::handlePostMakeMeeting(http_request request) {
    request.extract_json().then([=](json::value body) {
        json::value response;
        if (bob->isBusy()) {
            response[DATA_PARAM] = json::value::object(
                    {{MSG_PARAM, json::value::string(ROBOT_BUSY_ERROR_MSG)}});
            request.reply(status_codes::Conflict, response);
            return;
        }

        std::cout << body["title"].size() << std::endl;

        response[DATA_PARAM] = json::value::object(
                {{MSG_PARAM, json::value::string(ARRANGING_MSG)}});
        request.reply(status_codes::OK, response);

        for (auto stop: cachedPlan) {
            std::cout << "going to: " << stop.getId() << std::endl;
            bob->navigateTo(stop.getId());
        }
    });
}


void RestServer::handlePost(http_request request) {
    auto path = request.request_uri().path();
    if (path == MAKE_MEETING_PATH) {
        handlePostMakeMeeting(request);
    } else {
        request.reply(status_codes::NotFound);
    }
}
