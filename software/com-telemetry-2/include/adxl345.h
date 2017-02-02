/*
 * adxl345.h - ADXL345 accelerometer support
 */

#ifndef ADXL345_H_
#define ADXL345_H_

#include <stdbool.h>
#include <stdint.h>

// Initializes the ADXL345 in sleep mode
bool accelInit();
// Reads the current acceleration in 3.9 mG/LSB units
bool accelRead(int32_t *x, int32_t *y, int32_t *z);
// Powers up the accelerometer and starts collecting data
bool accelResume();
// Powers down the accelerometer, reducing current consumption to <1 uA but stopping measurement
bool accelSleep();

#endif
