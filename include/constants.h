#ifndef COORDIBOT_CONSTANTS_H
#define COORDIBOT_CONSTANTS_H


// Server

#define SERVER_PORT 6665
#define SERVER_IP "localhost"


// Rotation

#define DEG_90 1.57079633;
#define MAX_ROTATION_SPEED 0.62;
#define ROTATION_SPEED_1 0.62;
#define ROTATION_SPEED_2 0.155;
#define ROTATION_SPEED_3 0.062;
#define ROTATION_SPEED_4 0.01;

#define ANGLE_PRECISION_1 0.2
#define ANGLE_PRECISION_2 0.07
#define ANGLE_PRECISION_3 0.04

// Distances

#define NEARSET_POINT_THRESHOLD 0.1

#define BUSY "busy"
#define AVAILABLE "available"
#define PROGRESS_PATH "/progress"
#define STATUS_PATH "/status"
#define MAKE_MEETING_PATH "/makeMeeting"
#define ARRANGE_MEETING_PATH "/arrangeMeeting"

#define INVITED_PARAM "invited"
#define ESTIMATED_TIME_PARAM "estimatedTime"
#define STATUS_PARAM "status"
#define PROGRESS_PARAM "progress"
#define MSG_PARAM "msg"
#define DATA_PARAM "data"
#define REQUESTER_ID_PARAM "requester_id"

#define INVITED_ERROR_MSG "Missing invited param"
#define REQUESTER_ID_ERROR_MSG "Missing requester_id param"

#define NONE_MEETING_ERROR_MSG "None meetings were set"
#define ROBOT_BUSY_ERROR_MSG "Robot is currently busy"
#define INVALID_IDS_MSG "One or more id's are invalid"
#define ARRANGING_MSG "Arranging meeting"


#define MAX_MOVEMENT_SPEED 0.8

#endif //COORDIBOT_CONSTANTS_H
