#include "../include/server.h"

RestServer::RestServer(Robot *bob_) : listener_("http://127.0.0.1:8080") {
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

double getPlanCost(double distance, int numOfInvitess) {
    return (ceil(distance / (MAX_MOVEMENT_SPEED) + numOfInvitess * 30)) / 60;
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
            double planCost = getPlanCost(planDistance, invitedParamsArray.size());
            response[DATA_PARAM] = json::value::object(
                    {{ESTIMATED_TIME_PARAM, planCost}});
            request.reply(status_codes::OK, response);
        } catch (const std::exception &e) {
            response[DATA_PARAM] = json::value::object(
                    {{MSG_PARAM, json::value::string("e.what()")}});
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
            int planSize = plan.size();
            progress = {0, planSize};
            for (auto stop: plan) {
                std::cout << "going to: " << stop.getId() << std::endl;
                int currentProgress = std::get<0>(progress);
                int res = bob->navigateTo(stop.getId());
                if (res < 0) {
                    progress = {-1, planSize};
                    return;
                }
                progress = {currentProgress + 1, planSize};
                if (stop.getId() < 1000) {
                    bob->outputVoiceMessage();
                }
            }
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
            {{PROGRESS_PARAM, std::get<0>(progress) / std::get<1>(progress)}});
    request.reply(status_codes::OK, response);
}
