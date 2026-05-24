#ifndef INC_WALLS_H_
#define INC_WALLS_H_

#include <stdint.h>

/* Tune these thresholds empirically: place the robot at one cell's distance
   from each wall and record the sensor reading, then set the threshold
   slightly below that value. */
#define WALL_THRESHOLD_FRONT  2000
#define WALL_THRESHOLD_LEFT   1400
#define WALL_THRESHOLD_RIGHT  1800

uint8_t wallFront(void);
uint8_t wallLeft(void);
uint8_t wallRight(void);

#endif /* INC_WALLS_H_ */
