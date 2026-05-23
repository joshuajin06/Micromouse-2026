/*
 * motors.c
 */

#include "main.h"
#include "motors.h"

float limitPWM(float pwm) {
    if (pwm > PWM_MAX)  return  PWM_MAX;
    if (pwm < -PWM_MAX) return -PWM_MAX;
    return pwm;
}

/*
 * Left motor: TIM4 CH3 = forward, CH4 = backward.
 * Always zero the inactive channel before setting the active one.
 */
void setMotorLPWM(float pwm) {
    pwm = limitPWM(pwm);
    if (pwm >= 0.0f) {
        TIM4->CCR4 = 0;
        TIM4->CCR3 = (uint32_t)(pwm * MAX_TIMER_COUNTS);
    } else {
        TIM4->CCR3 = 0;
        TIM4->CCR4 = (uint32_t)(-pwm * MAX_TIMER_COUNTS);
    }
}

/*
 * Right motor: TIM4 CH1 = forward, CH2 = backward.
 * Always zero the inactive channel before setting the active one.
 */
void setMotorRPWM(float pwm) {
    pwm = limitPWM(pwm);
    if (pwm >= 0.0f) {
        TIM4->CCR2 = 0;
        TIM4->CCR1 = (uint32_t)(pwm * MAX_TIMER_COUNTS);
    } else {
        TIM4->CCR1 = 0;
        TIM4->CCR2 = (uint32_t)(-pwm * MAX_TIMER_COUNTS);
    }
}

void resetMotors(void) {
    setMotorLPWM(0.0f);
    setMotorRPWM(0.0f);
}
