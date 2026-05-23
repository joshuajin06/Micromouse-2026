/*
 * motors.h
 */

#ifndef INC_MOTORS_H_
#define INC_MOTORS_H_

#include "main.h"

/*
 * MAX_TIMER_COUNTS must match TIM4's auto-reload register (ARR) value set in CubeMX.
 * PWM_MAX caps the duty cycle to prevent overdriving the H-bridge.
 */
#define MAX_TIMER_COUNTS  3199u
#define PWM_MAX           1.0f

float limitPWM(float pwm);
void  setMotorLPWM(float pwm);
void  setMotorRPWM(float pwm);
void  resetMotors(void);

#endif /* INC_MOTORS_H_ */
