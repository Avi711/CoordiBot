#ifndef COORDIBOT_CONSTANTS_H
#define COORDIBOT_CONSTANTS_H

#define DEG_90 1.57079633;
#define MAX_ROTATION_SPEED 0.62;
#define ROTATION_SPEED_1 0.62;
#define ROTATION_SPEED_2 0.155;
#define ROTATION_SPEED_3 0.062;
#define ROTATION_SPEED_4 0.01;

#define ANGLE_PRECISION_1 0.2
#define ANGLE_PRECISION_2 0.07
#define ANGLE_PRECISION_3 0.04

#define NEARSET_POINT_THRESHOLD 0.1

#define BUSY "busy"
#define AVAILABLE "available"

#define STATUS_PATH "/status"
#define MAKE_MEETING_PATH "/makeMeeting"

#define INVITED_PARAM "invited"
#define ESTIMATED_TIME_PARAM "estimatedTime"
#define STATUS_PARAM "status"
#define MSG_PARAM "msg"
#define DATA_PARAM "data"

#define INVITED_ERROR_MSG "Missing invited param"
#define ROBOT_BUSY_ERROR_MSG "Robot is currently busy"
#define ARRANGING_MSG "Arranging meeting"

#define MAX_MOVEMENT_SPEED 1;


#endif //COORDIBOT_CONSTANTS_H
