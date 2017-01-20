/*
 * bme280.h - BME280 weather sensor driver
 */

#ifndef BME280_H_
#define BME280_H_

#include <stdbool.h>
#include <stdint.h>

// Initializes the BME280 sensor in sleep mode
bool bme280Init();
// Starts a BME280 measurement, the time taken is related to the oversampling factor
// T(measure,max) = 2.65 + 2.3 * TMP_OVR + 2.3 * P_OVR + 2.3 * H_OVR
bool bme280Measure();
// Reads weather data from the BME280
//  Temperature is reported as degrees C * 100
//  Humidity is reported as % RH * 100
//  Pressure is reported in Pa
bool bme280Read(int32_t *temperature, uint32_t *humidity, uint32_t *pressure);

#endif
