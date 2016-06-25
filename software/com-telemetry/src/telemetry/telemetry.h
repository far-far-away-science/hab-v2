#pragma once

typedef struct Telemetry_t
{
    uint16_t batteryVoltage;
    uint16_t cpuTemperature;
    uint16_t gpsChipTemperature;
    uint16_t batteryTemperature;
    uint16_t maxAccelerationX;
    uint16_t maxAccelerationY;
    uint16_t maxAccelerationZ;
    uint16_t outsidePressure;
    uint16_t outsideTemperature;
} Telemetry;
