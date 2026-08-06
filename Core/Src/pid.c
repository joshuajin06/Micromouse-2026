#include "main.h"
#include "motors.h"
#include "encoders.h"
#include "walls.h"

extern volatile uint16_t irLeft;
extern volatile uint16_t irRight;
#define cachedIrLeft  irLeft
#define cachedIrRight irRight

/* Tuning constants */
static float angleError = 0.0f;
static float oldAngleError = 0.0f;
static float distanceError = 0.0f;
static float oldDistanceError = 0.0f;
float kPw = 0.07f;
float kDw = 0.75f;
float kPx = 0.03f;
float kDx = 0.0f;

float kPir = 0.07f;
float kDir = 0.7f;

static float irError = 0.0f;
static float oldIrError = 0.0f;

static float goalAngle = 0.0f;
static float goalDistance = 0.0f;
static float gDistanceCorrection;
static float gAngleCorrection;

volatile int16_t dbg_finalLeft;
volatile int16_t dbg_finalRight;

#define DIST_THRESHOLD 2
#define ANGLE_THRESHOLD 2

#define IR_LEFT_CENTER 2000
#define IR_RIGHT_CENTER 1300

int isDone = 0;
int pidActive = 0;

void resetPID(void) {
    oldAngleError = 0;
    angleError = 0;
    distanceError = 0.0f;
    oldDistanceError = 0.0f;
    goalAngle = 0;
    goalDistance = 0;
    isDone = 0;
    resetEncoders();
    pidActive = 1;
}

void updatePID(void) {
    if(!pidActive) return;

    int16_t leftCounts  = getLeftEncoderCounts();
    int16_t rightCounts = getRightEncoderCounts();

    float leftError  = (float)cachedIrLeft  - IR_LEFT_CENTER;
    float rightError = (float)cachedIrRight - IR_RIGHT_CENTER;

    if (cachedIrLeft > WALL_THRESHOLD_LEFT && cachedIrRight > WALL_THRESHOLD_RIGHT) {
        irError = rightError - leftError;
    } else if (cachedIrLeft > WALL_THRESHOLD_LEFT) {
        irError = -leftError;
    } else if (cachedIrRight > WALL_THRESHOLD_RIGHT) {
        irError =  rightError;
    }

    oldIrError = irError;

    angleError = goalAngle - (float)(leftCounts - rightCounts);

    if(goalDistance != 0) {
      angleError -= kPir * irError + kDir * (irError - oldIrError);
    }

    float angleCorrection = kPw * angleError + kDw * (angleError - oldAngleError);
    // if (angleCorrection >  0.5f) angleCorrection =  0.5f;
    // if (angleCorrection < -0.5f) angleCorrection = -0.5f;
    gAngleCorrection = angleCorrection;
    oldAngleError = angleError;

    distanceError = goalDistance - ((float)(leftCounts + rightCounts)/2);
    float distanceCorrection = kPx * distanceError + kDx * (distanceError - oldDistanceError);
    if (distanceCorrection >  0.5f) distanceCorrection =  0.5f;
    if (distanceCorrection < -0.5f) distanceCorrection = -0.5f;
    oldDistanceError = distanceError;
    gDistanceCorrection = distanceCorrection;

    setMotorLPWM(distanceCorrection + angleCorrection);
    setMotorRPWM(distanceCorrection - angleCorrection);


    if (goalAngle == 0.0f && goalDistance != 0.0f) {
      // move: done on distance only
      if (distanceError > -DIST_THRESHOLD && distanceError < DIST_THRESHOLD) {
        dbg_finalLeft  = leftCounts;
        dbg_finalRight = rightCounts;
        isDone = 1;
      }
    } else if (goalDistance == 0.0f && goalAngle != 0.0f) {
      // turn: done on angle only
      if (angleError > -ANGLE_THRESHOLD && angleError < ANGLE_THRESHOLD) {
        dbg_finalLeft  = leftCounts;
        dbg_finalRight = rightCounts;
        isDone = 1;
      }
    }
}

void setPIDGoalD(int16_t distance) {
    goalDistance = distance;
    pidActive    = 1;
}

void setPIDGoalA(int16_t angle) {
    goalAngle = angle;
    pidActive = 1;
}

int8_t PIDdone(void) {
    return isDone;
}
