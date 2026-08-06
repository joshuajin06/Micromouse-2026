#ifndef INC_WALLS_H_
#define INC_WALLS_H_

#include <stdint.h>

/* Tune these thresholds empirically: place the robot at one cell's distance
   from each wall and record the sensor reading, then set the threshold
   slightly below that value. */
#define WALL_THRESHOLD_FRONT  1800
#define WALL_APPROACHING_FRONT 800
#define WALL_THRESHOLD_LEFT   1300
#define WALL_THRESHOLD_RIGHT  1100

uint8_t wallFront(void);
uint8_t wallLeft(void);
uint8_t wallRight(void);
uint8_t wallApproaching(void);

#endif /* INC_WALLS_H_ */
