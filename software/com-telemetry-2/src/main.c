/*
 * main.c - Telmetry main code
 */

#include <audio.h>
#include <bkup.h>
#include <main.h>
#include <periph.h>
#include <stdlib.h>
#include <string.h>
#include <stmtime.h>

// Main program
int main(void) {
	while (1) {
	}
	return 0;
}

// Fires every 10 ms to update system timer
void ISR_SysTick() {
	sysTime++;
}
