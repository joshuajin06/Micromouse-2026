/*
 * encoders.c
 */

#include "main.h"
#include "encoders.h"

/* TIM2 counts the right wheel encoder, TIM1 counts the left wheel encoder. */

int16_t getRightEncoderCounts(void) {
    return (int16_t)TIM2->CNT;
}

int16_t getLeftEncoderCounts(void) {
    return (int16_t)TIM1->CNT;
}

void resetEncoders(void) {
    TIM1->CNT = (int16_t)0;
    TIM2->CNT = (int16_t)0;
}
