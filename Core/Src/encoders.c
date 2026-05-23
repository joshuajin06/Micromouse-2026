/*
 * encoders.c
 */

#include "main.h"
#include "encoders.h"

/*
 * TIM1 (left)  is a 16-bit timer — direct int16_t cast is correct.
 * TIM2 (right) is a 32-bit timer with ARR=65535. Casting the 32-bit CNT
 * directly to int16_t truncates incorrectly when the counter wraps past
 * 65535 (e.g. backward from 0 → CNT=65535 → wrong sign). The fix is to
 * truncate to uint16_t first so the 16-bit two's-complement value is
 * preserved before interpreting as signed.
 */

int16_t getLeftEncoderCounts(void) {
    return (int16_t)TIM1->CNT;
}

int16_t getRightEncoderCounts(void) {
    return -(int16_t)(uint16_t)TIM2->CNT;
}

void resetEncoders(void) {
    TIM1->CNT = 0;
    TIM2->CNT = 0;
}
