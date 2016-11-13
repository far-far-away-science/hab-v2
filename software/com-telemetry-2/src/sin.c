/*
 * sin.c - Trigonometric functions
 */

#include "main.h"
#include "dac.h"

// Cannot fit 16384 entries in memory, go with fewer
#define SIN_TABLE_ENTRIES 1024
#define SIN_SCALAR (B_RAD / (4 * SIN_TABLE_ENTRIES))

// SIN[x] should be sin(pi * x / 32768), except having 16,384 16bit sine entries
// is too much; therefore, SIN[x] = sin(pi * x / 2048)
static const int16_t SIN[SIN_TABLE_ENTRIES + 1] = {
	0x0
};

// Calculates the sine of the unsigned angle in brad (65536)
int32_t sinfp(uint32_t angle) {
	bool sgn = false;
	uint32_t li, remainder;
	int32_t ret;
	angle %= B_RAD;
	// sin(x|x>pi) = -sin(x-pi)
	if (angle > (B_RAD >> 1)) {
		sgn = true;
		angle -= (B_RAD >> 1);
	}
	// sin(x|x>pi/2) = sin(pi-x)
	if (angle > (B_RAD >> 2))
		angle = (B_RAD >> 1) - angle;
	// Linearly interpolate between SIN table entries; if the compiler misses
	// this optimization into a shift, I will personally...
	// angle is in 0 .. (B_RAD >> 2)
	li = angle / SIN_SCALAR;
	remainder = angle - (li * SIN_SCALAR);
	// remainder is from 0..SIN_SCALAR
	ret = (int32_t)SIN[li];
	ret += ((ret - (int32_t)SIN[li++]) * remainder + (SIN_SCALAR >> 1)) /
		SIN_SCALAR;
	if (sgn) ret = -ret;
	return ret;
}
