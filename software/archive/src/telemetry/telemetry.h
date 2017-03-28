#pragma once

#include <stdint.h>

typedef struct Telemetry_t
{
    uint16_t batteryVoltage;
    uint16_t cpuTemperature;
    uint16_t gpsChipTemperature;
    uint16_t batteryTemperature;
    uint16_t maxAccelerationOnAnyAxis;
} Telemetry;
