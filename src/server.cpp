#include "../include/server.h"

RestServer::RestServer(Robot *bob_) : listener_(SERVER_IP) {
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

double getPlanCost(double distance, int numOfInvitess, std::vector<Vertex> route) {
    double travel_distance = (distance / (MAX_MOVEMENT_SPEED) + numOfInvitess * 30);
    double turning_cost = 0;
    auto prev = route.begin();
    auto current = prev + 1;
    while (current != route.end()) {
        double deg = abs(getDegree({current->getX(), current->getY()}, {prev->getX(), prev->getY()}));
        turning_cost += deg / ROTATION_SPEED_1 + 0.4;
        prev += 1;
        current += 1;
    }
    return ceil(travel_distance + turning_cost);
}

void RestServer::handlePostArrangeMeeting(http_request request) {
    request.extract_json().then([=](json::value body) {
        json::value response;
        try {
            if (bob->isBusy()) {
                response[DATA_PARAM] = json::value::object(
                        {{MSG_PARAM, json::value::string(ROBOT_BUSY_ERROR_MSG)}});
                request.reply(status_codes::Conflict, response);
                return;
            }
            if (!body.has_field(INVITED_PARAM)) {
                response[DATA_PARAM] = json::value::object(
                        {{MSG_PARAM, json::value::string(INVITED_ERROR_MSG)}});
                request.reply(status_codes::BadRequest, response);
                return;
            }
            if (!body.has_field(REQUESTER_ID_PARAM)) {
                response[DATA_PARAM] = json::value::object(
                        {{MSG_PARAM, json::value::string(REQUESTER_ID_ERROR_MSG)}});
                request.reply(status_codes::BadRequest, response);
                return;
            }
            std::vector<int> invitedParams;
            const auto &invitedParamsArray = body[INVITED_PARAM].as_array();
            for (const auto &element: invitedParamsArray) {
                invitedParams.push_back(element.as_integer());
            }
            Position cur_pos = bob->getPos();
            Vertex cur_vertex(cur_pos.getX(), cur_pos.getY());
            auto mp = bob->getMap();
            auto start = *getNearestStop(cur_vertex, *mp);
            auto route = getBestPlan(start, invitedParams, mp);
            if (std::get<1>(route) < 0) {
                response[DATA_PARAM] = json::value::object(
                        {{MSG_PARAM, json::value::string(INVALID_IDS_MSG)}});
                request.reply(status_codes::BadRequest, response);
                return;
            }
            cachedPlans[body[REQUESTER_ID_PARAM].as_string()] = std::get<0>(route);
            double planDistance = std::get<1>(route) + getDistance(cur_vertex, start);
            double planCost = getPlanCost(planDistance, invitedParamsArray.size(), std::get<0>(route));
            response[DATA_PARAM] = json::value::object(
                    {{ESTIMATED_TIME_PARAM, planCost}});
            request.reply(status_codes::OK, response);
        } catch (const std::exception &e) {
            response[DATA_PARAM] = json::value::object(
                    {{MSG_PARAM, json::value::string(e.what())}});
            request.reply(status_codes::Gone, response);
        }
    });
}

void RestServer::handleGet(http_request request) {
    auto path = request.request_uri().path();
    if (path == STATUS_PATH) {
        handleGetStatus(request);
    } else if (path == PROGRESS_PATH) {
        handleGetProgress(request);
    } else if (path == NOTIFIED_PATH) {
        handleNotified(request);
    } else {
        handleNotFound(request);
    }
}

void RestServer::handlePostMakeMeeting(http_request request) {
    std::cout << "handlePostMakeMeeting";
    request.extract_json().then([=](json::value body) {
        json::value response;
        if (bob->isBusy()) {
            response[DATA_PARAM] = json::value::object(
                    {{MSG_PARAM, json::value::string(ROBOT_BUSY_ERROR_MSG)}});
            request.reply(status_codes::Conflict, response);
            return;
        }

        std::string requester_id = body[REQUESTER_ID_PARAM].as_string();
        auto it = cachedPlans.find(requester_id);
        if (it != cachedPlans.end()) {
            response[DATA_PARAM] = json::value::object(
                    {{MSG_PARAM, json::value::string(ARRANGING_MSG)}});
            request.reply(status_codes::OK, response);
            auto plan = it->second;
            int planSize = plan.size() - 1;
            progress = {0, planSize};
            int flag = 0;
            notified = "";
            for (auto stop: plan) {
                std::cout << "going to: " << stop.getId() << std::endl;
                int currentProgress = std::get<0>(progress);
                int res = bob->navigateTo(stop.getId());
                if (res < 0) {
                    progress = {-1, planSize};
                    return;
                }
                notified += (std::to_string(stop.getId()) + ",");
                if (flag == 1)
                    progress = {currentProgress + 1, planSize};
                if (stop.getId() < 1000 && stop.getId() > 99) {
                    //bob->outputVoiceMessage();
                }
                flag = 1;
            }
            cachedPlans.clear();
        } else {
            response[DATA_PARAM] = json::value::object(
                    {{MSG_PARAM, json::value::string(NONE_MEETING_ERROR_MSG)}});
            request.reply(status_codes::BadRequest, response);
        }
    });
}


void RestServer::handlePost(http_request request) {
    auto path = request.request_uri().path();
    if (path == MAKE_MEETING_PATH) {
        handlePostMakeMeeting(request);
    } else if (path == ARRANGE_MEETING_PATH) {
        handlePostArrangeMeeting(request);
    } else {
        request.reply(status_codes::NotFound);
    }
}

void RestServer::handleGetProgress(http_request request) {
    json::value response;
    response[DATA_PARAM] = json::value::object(
            {{PROGRESS_PARAM, ceil((std::get<0>(progress) / std::get<1>(progress) * 100))}});
    request.reply(status_codes::OK, response);
}

void RestServer::handleNotified(http_request request) {
    json::value response;
    response[DATA_PARAM] = json::value::object(
            {{NOTIFIED_PARAM, json::value::string(notified)}});
    request.reply(status_codes::OK, response);
}
