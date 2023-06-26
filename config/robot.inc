define bigbobs_sonar sensor
(
    #define the size of each transducer [xsize ysize zsize] in meters
    size [0.01 0.05 0.01]
    #define the range bounds [min max]
    range [0 2.0]
    #define the angular field of view (by default in degrees)
    fov 30
    #define the color that ranges are drawn in the gui
    color_rgba [ 0 1 0 0.5]
)

define bigbobs_sonars ranger
(
  #one line for each sonar [xpos ypos zpos heading]
  bigbobs_sonar( pose [ 0.25 0 0 0])        # front
  bigbobs_sonar( pose [ 0.125 0.25 0 30])   # left corner
  bigbobs_sonar( pose [ 0.125 -0.25 0 -30]) # right corner
)

define bigbob position
(
    #actual size
    size [0.5 0.5 0.1]
    block(
            points 6
            point[0] [0.375 0]
            point[1] [0.5 0.125]
            point[2] [0.5 0.375]
            point[3] [0.375 0.5]
            point[4] [0 0.5]
            point[5] [0 0]
            z [0 0.5]


    )
    drive "diff"
    #sensors attached to bigbob
    bigbobs_sonars( ) # pose [0 0 0 0] is default

)
