/*
 * bme280.c - BME280 weather sensor driver
 */

#include <bme280.h>
#include <periph.h>
#include <stmtime.h>

// BME280 I2C address if SDO is LOW
#define BME280_I2C_ADDR_L 0x76
// BME280 I2C address if SDO is HIGH
#define BME280_I2C_ADDR_H 0x77

// BME280 registers

// Measurement control register
#define BME280_REG_CONTROL 0xF4
// Humidity measurement control register
#define BME280_REG_CONTROL_HUM 0xF2
// Device configuration register
#define BME280_REG_CONFIG 0xF5
// Device ID register
#define BME280_REG_CHIPID 0xD0
// Soft reset register
#define BME280_REG_SOFTRESET 0xE0
// Calibration value 26 (up to 41 at register 0xF0)
#define BME280_REG_CAL26 0xE1
// Temperature most significant byte, big endian
#define BME280_REG_TEMP 0xFA
// Pressure most significant byte, big endian
#define BME280_REG_PRESS 0xF7
// Humidity most significant byte, big endian
#define BME280_REG_HUM 0xFD
// Calibration value 00 (up to 25 at 0xA1)
#define BME280_REG_DIG_T 0x88
// Calibration value 07
#define BME280_REG_DIG_P 0x8E
// Calibration value 25
#define BME280_REG_DIG_H1 0xA1
// Calibration value 26
#define BME280_REG_DIG_H2 0xE1

// BME280 power mode constants

// Sleep mode
#define BME280_SLEEP_MODE 0x00
// Forced mode
#define BME280_FORCED_MODE 0x01
// Normal mode
#define BME280_NORMAL_MODE 0x03
// Writable to BME280_REG_SOFTRESET to reset the device
#define BME280_SOFT_RESET_CODE 0xB6

// BME280 oversampling constants

#define BME280_HUM_POS 0
#define BME280_PRS_POS 2
#define BME280_TMP_POS 5

// 1x oversampling
#define BME280_OVERSAMP_1X 0x01
#define BME280_HUM_OVR_1X (BME280_OVERSAMP_1X << BME280_HUM_POS)
#define BME280_PRS_OVR_1X (BME280_OVERSAMP_1X << BME280_PRS_POS)
#define BME280_TMP_OVR_1X (BME280_OVERSAMP_1X << BME280_TMP_POS)
// 2x oversampling
#define BME280_OVERSAMP_2X 0x02
#define BME280_HUM_OVR_2X (BME280_OVERSAMP_2X << BME280_HUM_POS)
#define BME280_PRS_OVR_2X (BME280_OVERSAMP_2X << BME280_PRS_POS)
#define BME280_TMP_OVR_2X (BME280_OVERSAMP_2X << BME280_TMP_POS)
// 4x oversampling
#define BME280_OVERSAMP_4X 0x03
#define BME280_HUM_OVR_4X (BME280_OVERSAMP_4X << BME280_HUM_POS)
#define BME280_PRS_OVR_4X (BME280_OVERSAMP_4X << BME280_PRS_POS)
#define BME280_TMP_OVR_4X (BME280_OVERSAMP_4X << BME280_TMP_POS)
// 8x oversampling
#define BME280_OVERSAMP_8X 0x04
#define BME280_HUM_OVR_8X (BME280_OVERSAMP_8X << BME280_HUM_POS)
#define BME280_PRS_OVR_8X (BME280_OVERSAMP_8X << BME280_PRS_POS)
#define BME280_TMP_OVR_8X (BME280_OVERSAMP_8X << BME280_TMP_POS)
// 16x oversampling
#define BME280_OVERSAMP_16X 0x05
#define BME280_HUM_OVR_16X (BME280_OVERSAMP_16X << BME280_HUM_POS)
#define BME280_PRS_OVR_16X (BME280_OVERSAMP_16X << BME280_PRS_POS)
#define BME280_TMP_OVR_16X (BME280_OVERSAMP_16X << BME280_TMP_POS)

// BME280 standby time constants

// 1ms standby
#define BME280_STANDBY_1_MS 0x00
// 63ms standby
#define BME280_STANDBY_63_MS 0x20
// 125ms standby
#define BME280_STANDBY_125_MS 0x40
// 250ms standby
#define BME280_STANDBY_250_MS 0x60
// 500ms standby
#define BME280_STANDBY_500_MS 0x80
// 1000ms standby
#define BME280_STANDBY_1000_MS 0xA0
// 10ms standby
#define BME280_STANDBY_10_MS 0xC0
// 20ms standby
#define BME280_STANDBY_20_MS 0xE0

// BME280 filter constants

// No filter
#define BME280_FILTER_OFF 0x00
// Filter coefficient of 2
#define BME280_FILTER_2 0x04
// Filter coefficient of 4
#define BME280_FILTER_4 0x08
// Filter coefficient of 8
#define BME280_FILTER_8 0x0C
// Filter coefficient of 16
#define BME280_FILTER_16 0x10

// BME280 mode constants

// Sleep mode
#define BME280_MODE_SLEEP 0x00
// Forced mode
#define BME280_MODE_FORCED 0x01
// Normal (infinite loop) mode
#define BME280_MODE_NORMAL 0x03


// Selected pressure/temperature oversampling goes in the control register along with the mode
#define BME280_CTRL (BME280_PRS_OVR_16X | BME280_TMP_OVR_16X)

// For the most part, matches BME280 memory layout exactly (little endian!)
static struct {
	// dig_T1 - 0x88..0x89 - unsigned short
	uint16_t t1;
	// dig_T2 - 0x8A..0x8B - signed short
	int16_t t2;
	// dig_T3 - 0x8C..0x8D - signed short
	int16_t t3;
	// dig_P1 - 0x8E..0x8F - unsigned short
	uint16_t p1;
	// dig_P2 - 0x90..0x91 - signed short
	int16_t p2;
	// dig_P3 - 0x92..0x93 - signed short
	int16_t p3;
	// dig_P4 - 0x94..0x95 - signed short
	int16_t p4;
	// dig_P5 - 0x96..0x97 - signed short
	int16_t p5;
	// dig_P6 - 0x98..0x99 - signed short
	int16_t p6;
	// dig_P7 - 0x9A..0x9B - signed short
	int16_t p7;
	// dig_P8 - 0x9C..0x9D - signed short
	int16_t p8;
	// dig_P9 - 0x9E..0x9F - signed short
	int16_t p9;
	// RESERVED - 0xA0
	uint8_t RESERVED;
	// dig_H1 - 0xA1 - unsigned char
	uint8_t h1;
	// dig_H2 - 0xE1..0xE2 - signed short
	int16_t h2;
	// dig_H3 - 0xE3 - unsigned char
	uint8_t h3;
	// dig_H4 - 0xE4..0xE5 - signed short
	int16_t h4;
	// dig_H5 - 0xE5..0xE6 - signed short
	int16_t h5;
	// dig_H6 - 0xE7 - signed char
	int8_t h6;
} PACKED __attribute__ ((aligned(4))) bme280Cal;

// Copied essentially verbatim from the BME280 datasheet page 23-24
#if 1

// Returns temperature in DegC, resolution is 0.01 DegC
// adcFine carries fine temperature to next method
static int32_t BME280_compensate_T(const int32_t adcTemp, int32_t *adcFine) {
	int32_t var1, var2, dig_T1 = (int32_t)bme280Cal.t1, tFine;
	var1 = ((((adcTemp >> 3) - (dig_T1 << 1))) * ((int32_t)bme280Cal.t2)) >> 11;
	var2 = (((((adcTemp >> 4) - dig_T1) * ((adcTemp >> 4) - dig_T1)) >> 12) *
		((int32_t)bme280Cal.t3)) >> 14;
	tFine = var1 + var2;
	*adcFine = tFine;
	return (tFine * 5 + 128) >> 8;
}

// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8
// fractional bits)
static uint32_t BME280_compensate_P(const int32_t adcPres, const int32_t tFine) {
	int64_t var1, var2, p = 0LL;
	var1 = (int64_t)tFine - 128000;
	var2 = var1 * var1 * (int64_t)bme280Cal.p6;
	var2 = var2 + ((var1 * (int64_t)bme280Cal.p5) << 17);
	var2 = var2 + ((int64_t)bme280Cal.p4 << 35);
	var1 = ((var1 * var1 * (int64_t)bme280Cal.p3) >> 8) + ((var1 * (int64_t)bme280Cal.p2) <<
		12);
	var1 = ((1LL << 47) + var1) * ((int64_t)bme280Cal.p1) >> 33;
	if (var1 != 0LL) {
		int32_t p13;
		// Avoid exception caused by division by zero
		p = 1048576 - adcPres;
		p = (((p << 31) - var2) * 3125) / var1;
		p13 = p >> 13;
		var1 = ((int64_t)bme280Cal.p9 * p13 * p13) >> 25;
		var2 = ((int64_t)bme280Cal.p8 * p) >> 19;
		p = ((p + var1 + var2) >> 8) + ((int64_t)bme280Cal.p7 << 4);
	}
	return (uint32_t)p;
}

// Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and 10
// fractional bits)
static uint32_t BME280_compensate_H(const int32_t adcHum, const int32_t tFine) {
	int32_t var1, var15;
	var1 = tFine - 76800;
	var1 = ((((adcHum << 14) - ((int32_t)bme280Cal.h4 << 20) - ((int32_t)bme280Cal.h5 * var1)) +
		16384) >> 15) * (((((((var1 * (int32_t)bme280Cal.h6) >> 10) * ((((int32_t)bme280Cal.h3 *
		var1) >> 11) + 32768)) >> 10) + 2097152) * (int32_t)bme280Cal.h2 + 8192) >> 14);
	var15 = var1 >> 15;
	var1 = var1 - ((((var15 * var15) >> 7) * (int32_t)bme280Cal.h1) >> 4);
	// Maintain sane range of 0..100% RH
	if (var1 < 0)
		var1 = 0;
	if (var1 > 419430400)
		var1 = 419430400;
	// Correct rounding!
	return (uint32_t)((var1 + 2048U) >> 12);
}

#endif

// Initializes the BME280 sensor in sleep mode
bool bme280Init() {
	uint8_t temp[4];
	// Correct version is 0x60 (datasheet page 25)
	bool ok = i2cReadRegister(BME280_I2C_ADDR_L, BME280_REG_CHIPID, &temp[0], 1) &&
		temp[0] == 0x60U;
	if (ok) {
		uint32_t e5;
		// Reset the chip (2ms startup time according to datasheet)
		ok = ok && i2cWriteRegister(BME280_I2C_ADDR_L, BME280_REG_SOFTRESET,
			BME280_SOFT_RESET_CODE);
		delay(1U);
		// Set filter off, no SPI 3 wire, 250ms standby (but we use forced mode anyways)
		ok = ok && i2cWriteRegister(BME280_I2C_ADDR_L, BME280_REG_CONFIG,
			BME280_STANDBY_250_MS | BME280_FILTER_OFF);
		// Must write humidity mode first, 4x oversampling for now
		ok = ok && i2cWriteRegister(BME280_I2C_ADDR_L, BME280_REG_CONTROL_HUM,
			BME280_HUM_OVR_16X);
		// 4x pressure and temperature oversampling, stay in sleep mode
		ok = ok && i2cWriteRegister(BME280_I2C_ADDR_L, BME280_REG_CONTROL, BME280_CTRL |
			BME280_MODE_SLEEP);
		// Read calibration data: T1..H1
		ok = ok && i2cReadRegister(BME280_I2C_ADDR_L, BME280_REG_DIG_T, &bme280Cal.t1, 26U);
		// Read calibration data: H2..H3
		ok = ok && i2cReadRegister(BME280_I2C_ADDR_L, BME280_REG_DIG_H2, &bme280Cal.h2, 3U);
		// Read calibration data: H4
		ok = ok && i2cReadRegister(BME280_I2C_ADDR_L, BME280_REG_DIG_H2 + 3, &temp[0], 4U);
		e5 = (uint32_t)temp[1];
		// 0xE4 | 0xE5 [3:0] = dig_H5 [11:4] | [3:0]
		bme280Cal.h4 = (int16_t)(((uint32_t)temp[0] << 4) | (e5 & 0x0FU));
		// 0xE5 [7:4] | 0xE6 = dig_H5 [3:0] | [11:4]
		bme280Cal.h5 = (int16_t)(((uint32_t)temp[1] << 4) | ((e5 & 0xF0U) >> 4));
		bme280Cal.h6 = (int8_t)temp[3];
	}
	return ok;
}

// Starts a BME280 measurement, the time taken is related to the oversampling factor
bool bme280Measure() {
	return i2cWriteRegister(BME280_I2C_ADDR_L, BME280_REG_CONTROL, BME280_CTRL |
		BME280_MODE_FORCED);
}

// Reads weather data from the BME280
bool bme280Read(int32_t *temperature, uint32_t *humidity, uint32_t *pressure) {
	uint8_t meas[8];
	bool ok = i2cReadRegister(BME280_I2C_ADDR_L, BME280_REG_PRESS, &meas[0], sizeof(meas));
	if (ok) {
		// Build up the values
		int32_t press = (int32_t)(((uint32_t)meas[0] << 12) | ((uint32_t)meas[1] << 4) |
			((uint32_t)meas[2] >> 4));
		int32_t temp = (int32_t)(((uint32_t)meas[3] << 12) | ((uint32_t)meas[4] << 4) |
			((uint32_t)meas[5] >> 4));
		int32_t hum = (int32_t)(((uint32_t)meas[6] << 8) | (uint32_t)meas[7]), tFine;
		// Pass to Bosch provided functions
		*temperature = BME280_compensate_T(temp, &tFine);
		*pressure = (BME280_compensate_P(press, tFine) + 128U) >> 8;
		*humidity = (100U * BME280_compensate_H(hum, tFine) + 512U) >> 10;
	}
	return ok;
}
