import math

trigMultiplier = 512
inverseTrigMultiplier = 1024

inverseTrigCount = 2 * inverseTrigMultiplier + 1
trigSamplesCount = int(math.pi * 2.0 * trigMultiplier + 0.5) + 1

QUANT_MIN_VALUE  = 0
QUANT_MAX_VALUE  = 4095
AMPLITUDE_SCALER = (QUANT_MAX_VALUE - QUANT_MIN_VALUE) / 2.0
AMPLITUDE_SHIFT  = AMPLITUDE_SCALER + QUANT_MIN_VALUE

print('#include "trigonometry.h"')
print()
print('#ifndef TRIG_SLOW')
print()
print('    const int TRIG_COUNT = ' + str(trigSamplesCount) + ';')
print('    const int INVERSE_TRIG_COUNT = ' + str(inverseTrigCount) + ';')
print('    const int TRIG_MULTIPLIER = ' + str(trigMultiplier) + ';')
print('    const int INVERSE_TRIG_MULTIPLIER = ' + str(inverseTrigMultiplier) + ';')
print()
print('    const float AMPLITUDE_SCALED_AND_SHIFTED_SINE_TABLE[] = {')
for i in range(trigSamplesCount):
    trigArg = float(i) / trigMultiplier
    value = round(AMPLITUDE_SHIFT + AMPLITUDE_SCALER * math.sin(trigArg), 8)
    print("        {0:<11.8}, // id = {1:04}, value = PI * {2:<11.8} = {3:<.8}".format(value,
                                                                                       i,
                                                                                       round(float(i) / (trigMultiplier * 2.0 * math.pi), 8),
                                                                                       round(trigArg, 8)))
print('    };')
print()
print('    const float ASIN[] = {')
for i in range(inverseTrigCount):
    value = 2.0 * i / inverseTrigCount - 1.0
    print("        {0:<11.8}, // id = {1:04}, value = {2:<.8}".format(round(math.asin(value), 8),
                                                                      i,
                                                                      round(value, 8)))
print('    };')
print()
print('    const float COS_G_THAN_0[] = {')
for i in range(trigSamplesCount):
    value = float(i) / trigMultiplier
    print("        {0:<5}, // id = {1:04}, value = PI * {2:<11.8} = {3:<.8}".format("true" if math.cos(value) >= 0 else "false",
                                                                                    i,
                                                                                    round(float(i) / (trigMultiplier * 2.0 * math.pi), 8),
                                                                                    round(value, 8)))
print('    };')
print()
print('#endif')
