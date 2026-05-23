/*
 * systick.c
 */

#include "main.h"
#include "pid.h"
#include "encoders.h"

void SysTickFunction(void) {
    /* Called every millisecond by the SysTick ISR */
    updatePID();

    /*
     * Prevents PID windup when encoder counts approach int16 overflow (~9 m of travel).
     * Preserves the left/right difference so angular PID is unaffected by the reset.
     */
    if (getRightEncoderCounts() > 31000 || getLeftEncoderCounts() > 31000
            || getRightEncoderCounts() < -31000 || getLeftEncoderCounts() < -31000) {
        int16_t difference = getRightEncoderCounts() - getLeftEncoderCounts();
        resetEncoders();
        TIM2->CNT = (int16_t) difference; //set right encoder counts to difference
    }
}
