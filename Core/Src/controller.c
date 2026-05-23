/*
 * controller.c
 */

#include "main.h"
#include "controller.h"
#include "pid.h"

/*
 * Encoder counts calibrated for your maze cell size and hardware.
 * Measure empirically: drive one cell, read encoder counts, set COUNTS_PER_CELL.
 * Similarly for COUNTS_PER_90DEG.
 */
#define COUNTS_PER_CELL    625  /* encoder counts for one 180mm maze cell */
#define COUNTS_PER_90DEG   562  /* encoder counts for a 90-degree in-place turn */

/*
 * move(1)  → forward one cell
 * move(-1) → backward one cell
 * move(n)  → n cells
 */
void move(int8_t n) {
    resetPID();
    setPIDGoalD((int16_t)(n * COUNTS_PER_CELL));
    setPIDGoalA(0);

    while (!PIDdone()) {
        /* SysTick ISR calls updatePID() every millisecond; just wait */
    }

    resetPID();
}

/*
 * turn(1)  → 90 degrees in positive rotation direction (e.g. right)
 * turn(-1) → 90 degrees in negative rotation direction (e.g. left)
 * turn(2)  → 180 degrees (U-turn)
 */
void turn(int8_t n) {
    resetPID();
    setPIDGoalD(0);
    setPIDGoalA((int16_t)(n * COUNTS_PER_90DEG));

    while (!PIDdone()) {
        /* SysTick ISR calls updatePID() every millisecond; just wait */
    }

    resetPID();
}
