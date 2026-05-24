/*
 * walls.c
 */

#include "walls.h"
#include "irs.h"

uint8_t wallFront(void)
{
    return readFrontIR() > WALL_THRESHOLD_FRONT;
}

uint8_t wallLeft(void)
{
    return readLeftIR() > WALL_THRESHOLD_LEFT;
}

uint8_t wallRight(void)
{
    return readRightIR() > WALL_THRESHOLD_RIGHT;
}
