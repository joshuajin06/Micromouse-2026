/*
 * pid.c
 */

#include "main.h"
#include "motors.h"
#include "encoders.h"

/* Tuning constants — adjust these for your hardware */
#define BASE_SPEED        0.30f   /* nominal duty cycle (0.0–1.0) */
#define KPW               0.005f  /* proportional gain, angular */
#define KDW               0.020f  /* derivative gain, angular */
#define KPD               0.0003f /* proportional gain, distance */
#define KDD               0.001f  /* derivative gain, distance */
#define DONE_THRESHOLD    10      /* encoder count band considered "at goal" */
#define DONE_CYCLES       50      /* consecutive SysTick cycles within band to be done */
#define ERROR_HISTORY     12      /* ring-buffer depth for derivative smoothing */

static int16_t goalDistance = 0;
static int16_t goalAngle    = 0;

static float oldDistanceError = 0.0f;
static float oldAngleError    = 0.0f;

static int16_t angleErrorHistory[ERROR_HISTORY];
static uint8_t historyIndex = 0;

static int16_t doneCounter  = 0;
static int8_t  isDone       = 0;
static int8_t  pidActive    = 0;

void resetPID(void) {
    goalDistance     = 0;
    goalAngle        = 0;
    oldDistanceError = 0.0f;
    oldAngleError    = 0.0f;
    doneCounter      = 0;
    isDone           = 0;
    pidActive        = 0;
    historyIndex     = 0;

    for (int i = 0; i < ERROR_HISTORY; i++) {
        angleErrorHistory[i] = 0;
    }

    resetEncoders();
    resetMotors();
}

void updatePID(void) {
    if (!pidActive) return;

    int16_t rightCounts = getRightEncoderCounts();
    int16_t leftCounts  = getLeftEncoderCounts();

    /* Positive angleError → right wheel ahead → rat drifting left */
    float angleError    = (float)(rightCounts - leftCounts) - (float)goalAngle;
    float distanceError = (float)goalDistance - (float)((rightCounts + leftCounts) / 2);

    /* Ring buffer: compare current error to the sample ERROR_HISTORY cycles ago */
    angleErrorHistory[historyIndex] = (int16_t)angleError;
    historyIndex = (uint8_t)((historyIndex + 1) % ERROR_HISTORY);

    float dAngle    = angleError - (float)angleErrorHistory[historyIndex];
    float dDistance = distanceError - oldDistanceError;

    float angularCorrection  = KPW * angleError    + KDW * dAngle;
    float distanceCorrection = KPD * distanceError + KDD * dDistance;

    float leftPWM  = BASE_SPEED + distanceCorrection - angularCorrection;
    float rightPWM = BASE_SPEED + distanceCorrection + angularCorrection;

    setMotorLPWM(leftPWM);
    setMotorRPWM(rightPWM);

    oldAngleError    = angleError;
    oldDistanceError = distanceError;

    if (distanceError > -DONE_THRESHOLD && distanceError < DONE_THRESHOLD &&
        angleError    > -DONE_THRESHOLD && angleError    < DONE_THRESHOLD) {
        doneCounter++;
        if (doneCounter >= DONE_CYCLES) {
            isDone = 1;
        }
    } else {
        doneCounter = 0;
        isDone      = 0;
    }
}

void setPIDGoalD(int16_t distance) {
    goalDistance = distance;
    pidActive = 1;
}

void setPIDGoalA(int16_t angle) {
    goalAngle = angle;
    pidActive = 1;
}

int8_t PIDdone(void) {
    return isDone;
}
