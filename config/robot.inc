define bigbobs_sonar sensor
(
    #define the size of each transducer [xsize ysize zsize] in meters
    size [0.01 0.05 0.01 ]
    #define the range bounds [min max]
    range [0.3 2.0]
    #define the angular field of view (by default in degrees)
    fov 50
    #define the color that ranges are drawn in the gui
    color_rgba [ 0 1 0 0.5]
)

define bigbobs_sonars ranger
(
  #one line for each sonar [xpos ypos zpos heading]
  bigbobs_sonar( pose [ 0 0.5 0 0])       # front
  bigbobs_sonar( pose [ 0.25 0.5 0 30])  # left corner
  bigbobs_sonar( pose [ 0.25 -0.5 0 -30]) # right corner
)


define bigbob position
(
    #actual size
    size [0.5 0.5 0.1]
    block(
        points 6
        point[0] [0.75 0]
        point[1] [1 0.25]
        point[2] [1 0.75]
        point[3] [0.75 1]
        point[4] [0 1]
        point[5] [0 0]
        z [0 1]
    )
    drive "diff"
    #sensors attached to bigbob
    bigbobs_sonars( ) # pose [0 0 0 0] is default
)
