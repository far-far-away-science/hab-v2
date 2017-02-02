/*
 * adxl345.c - ADXL345 accelerometer support
 */

#include <adxl345.h>
#include <periph.h>

// ADXL345 I2C address if SDO is LOW (shifted left one for STM32!)
#define ADXL345_I2C_ADDR_L 0xA6
// ADXL345 I2C address if SDO is HIGH (shifted left one for STM32!)
#define ADXL345_I2C_ADDR_H 0x3A

// ADXL345 registers

// ID register
#define ADXL_REG_ID 0x00
// Tap threshold register
#define ADXL_REG_THRESH_TAP 0x1D
// User offset register for X axis, 15.6 mG/LSB
#define ADXL_REG_OFSX 0x1E
// User offset register for Y axis, 15.6 mG/LSB
#define ADXL_REG_OFSY 0x1F
// User offset register for Z axis, 15.6 mG/LSB
#define ADXL_REG_OFSZ 0x20
// Tap duration register, 625 uS/LSB
#define ADXL_REG_DUR 0x21
// Tap separation register, 1.25 mS/LSB
#define ADXL_REG_LATENT 0x22
// Double tap window register, 1.25 mS/LSB
#define ADXL_REG_WINDOW 0x23
// Activity threshold register, 62.5 mG/LSB
#define ADXL_REG_THRESH_ACT 0x24
// Inactivity threshold register, 62.5 mG/LSB
#define ADXL_REG_THRESH_INACT 0x25
// Inactivity minimum duration register, 1 s/LSB
#define ADXL_REG_TIME_INACT 0x26
// Activity and inactivity control register
#define ADXL_REG_ACT_INACT_CTL 0x27
// Free fall composite threshold register, 62.5 mG/LSB
#define ADXL_REG_THRES_FF 0x28
// Free fall duration register, 5 ms/LSB
#define ADXL_REG_TIME_FF 0x29
// Tap axis selection register
#define ADXL_REG_TAP_AXES 0x2A
// Activity and tap status register
#define ADXL_REG_ACT_TAP_STATUS 0x2B
// Bandwidth and reduced rate control register
#define ADXL_REG_BW_RATE 0x2C
// Power control register
#define ADXL_REG_POWER_CTL 0x2D
// Interrupt enable register
#define ADXL_REG_INT_ENABLE 0x2E
// Interrupt map register
#define ADXL_REG_INT_MAP 0x2F
// Interrupt source register
#define ADXL_REG_INT_SOURCE 0x30
// Data format and range register
#define ADXL_REG_DATA_FORMAT 0x31
// X acceleration, 2 bytes LSB first
#define ADXL_REG_DATA_X_L 0x32
#define ADXL_REG_DATA_X_H 0x33
// Y acceleration, 2 bytes LSB first
#define ADXL_REG_DATA_Y_L 0x34
#define ADXL_REG_DATA_Y_H 0x35
// Z acceleration, 2 bytes LSB first
#define ADXL_REG_DATA_Z_L 0x36
#define ADXL_REG_DATA_Z_H 0x37
// FIFO mode register
#define ADXL_REG_FIFO_MODE 0x38
// FIFO status register
#define ADXL_REG_FIFO_STATUS 0x39

// Activity detection AC coupling or DC coupling selection
#define ADXL_ACT_AC 0x80
#define ADXL_ACT_DC 0x00
// Activity detection axis enable
#define ADXL_ACT_X_EN 0x40
#define ADXL_ACT_Y_EN 0x20
#define ADXL_ACT_Z_EN 0x10
// Inactivity detection AC coupling or DC coupling selection
#define ADXL_INACT_AC 0x08
#define ADXL_INACT_DC 0x00
// Inactivity detection axis enable
#define ADXL_INACT_X_EN 0x04
#define ADXL_INACT_Y_EN 0x02
#define ADXL_INACT_Z_EN 0x01

// Suppresses double tap detection if |acceleration| > THRESH_TAP is present between taps
#define ADXL_TAP_SUPPRESS 0x08
// Tap detection axis enable
#define ADXL_TAP_X_EN 0x04
#define ADXL_TAP_Y_EN 0x02
#define ADXL_TAP_Z_EN 0x01

// Activity axis source flags
#define ADXL_ACT_X_SRC 0x40
#define ADXL_ACT_Y_SRC 0x20
#define ADXL_ACT_Z_SRC 0x10
// Set if device is in sleep mode
#define ADXL_ASLEEP 0x08
// Tap axis source flags
#define ADXL_TAP_X_SRC 0x04
#define ADXL_TAP_Y_SRC 0x02
#define ADXL_TAP_Z_SRC 0x01

// Enables low power mode
#define ADXL_LOW_POWER 0x10
// Rate selection constants
#define ADXL_RATE_3200 0x0F
#define ADXL_RATE_1600 0x0E
#define ADXL_RATE_800 0x0D
#define ADXL_RATE_400 0x0C
#define ADXL_RATE_200 0x0B
#define ADXL_RATE_100 0x0A
#define ADXL_RATE_50 0x09
#define ADXL_RATE_25 0x08
#define ADXL_RATE_12P5 0x07
#define ADXL_RATE_6P25 0x06

// Link inactivity and activity events serially
#define ADXL_LINK 0x20
// Switches into sleep mode automatically when inactivity detected
#define ADXL_AUTO_SLEEP 0x10
// Enables measurement
#define ADXL_MEASURE 0x08
// Enables sleep mode
#define ADXL_SLEEP 0x04
// Sampling rate in sleep mode selection constants
#define ADXL_WAKEUP_8 0x00
#define ADXL_WAKEUP_4 0x01
#define ADXL_WAKEUP_2 0x02
#define ADXL_WAKEUP_1 0x03

// These constants apply to INT_ENABLE, INT_MAP, and INT_SOURCE

// Enables interrupt on data ready
#define ADXL_INT_DATA_READY 0x80
// Enables interrupt on single tap
#define ADXL_INT_SINGLE_TAP 0x40
// Enables interrupt on double tap
#define ADXL_INT_DOUBLE_TAP 0x20
// Enables interrupt on activity
#define ADXL_INT_ACTIVITY 0x10
// Enables interrupt on inactivity
#define ADXL_INT_INACTIVITY 0x08
// Enables interrupt on free fall detection
#define ADXL_INT_FREEFALL 0x04
// Enables interrupt on FIFO watermark
#define ADXL_INT_WATERMARK 0x02
// Enables interrupt on FIFO overrun
#define ADXL_INT_OVERRUN 0x01

// Enable self test
#define ADXL_SELF_TEST 0x80
// Enable 3 wire SPI mode
#define ADXL_SPI 0x40
// Set interrupts to active low
#define ADXL_INT_INVERT 0x20
// Enable full resolution (always 4 mg/LSB, as opposed to fixed 10-bit) mode
#define ADXL_FULL_RES 0x08
// Enable left justified mode
#define ADXL_JUSTIFY 0x04
// Full-scale range selection constants
#define ADXL_FS_2G 0x00
#define ADXL_FS_4G 0x01
#define ADXL_FS_8G 0x02
#define ADXL_FS_16G 0x03

// Bypasses the FIFO
#define ADXL_FIFO_BYPASS 0x00
// Collect until FIFO is full and then stop
#define ADXL_FIFO_FIFO 0x40
// Collect values, discarding the oldest
#define ADXL_FIFO_STREAM 0x80
// Only collects data when triggered and then stops
#define ADXL_FIFO_TRIGGER 0xC0
// Trigger interrupt selection constants
#define ADXL_TRIGGER_INT1 0x00
#define ADXL_TRIGGER_INT2 0x20
// The lowest 5 bits are the quantity of samples to trigger a watermark interrupt

// FIFO trigger flag
#define ADXL_FIFO_TRIG 0x80
// The lowest 6 bits are the number of data values in the FIFO

// Initializes the ADXL345 in sleep mode
bool accelInit() {
	uint8_t temp;
	bool ok = i2cReadRegister(ADXL345_I2C_ADDR_L, 0x00, &temp, 1U) && temp == 0xE5U;
	if (ok) {
		// Power down accelerometer, turn off sleep mode, select 1 Hz in sleep mode
		ok = ok && accelSleep();
		// Disable low power mode (accelerometer will be put to sleep on the ground), 50 Hz
		ok = ok && i2cWriteRegister(ADXL345_I2C_ADDR_L, ADXL_REG_BW_RATE, ADXL_RATE_50);
		// Disable interrupts
		ok = ok && i2cWriteRegister(ADXL345_I2C_ADDR_L, ADXL_REG_INT_ENABLE, 0U);
		// Full resolution mode, 16 g max deflection (no loss in not doing so!)
		ok = ok && i2cWriteRegister(ADXL345_I2C_ADDR_L, ADXL_REG_DATA_FORMAT, ADXL_FULL_RES |
			ADXL_FS_16G);
		// Disable the FIFO (due to our low sampling rate), set a 16 sample level for watermark
		ok = ok && i2cWriteRegister(ADXL345_I2C_ADDR_L, ADXL_REG_FIFO_MODE, ADXL_FIFO_BYPASS |
			16U);
	}
	return ok;
}

// Reads the current acceleration in 3.9 mG/LSB units
bool accelRead(int32_t *x, int32_t *y, int32_t *z) {
	uint8_t temp[6];
	const bool ok = i2cReadRegister(ADXL345_I2C_ADDR_L, ADXL_REG_DATA_X_L, &temp[0], 6U);
	if (ok) {
		// Convert data little endian right justified
		*x = (int32_t)temp[0] | ((int32_t)(int8_t)temp[1] << 8);
		*y = (int32_t)temp[2] | ((int32_t)(int8_t)temp[3] << 8);
		*z = (int32_t)temp[4] | ((int32_t)(int8_t)temp[5] << 8);
	}
	return ok;
}

// Powers up the accelerometer and starts collecting data
bool accelResume() {
	return i2cWriteRegister(ADXL345_I2C_ADDR_L, ADXL_REG_POWER_CTL, ADXL_WAKEUP_1 | ADXL_MEASURE);
}

// Powers down the accelerometer, reducing current consumption to <1 uA but stopping measurement
bool accelSleep() {
	return i2cWriteRegister(ADXL345_I2C_ADDR_L, ADXL_REG_POWER_CTL, ADXL_WAKEUP_1);
}
