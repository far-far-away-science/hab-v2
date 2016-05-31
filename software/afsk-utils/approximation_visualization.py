import numpy
import random
import matplotlib.pyplot as plot

import afsk_modulation
import data_generation
import definitions_derived
import afsk_modulation_fixedpoint

bitsCount = int(128 * 8 + 128 / 5 * 8 + 1)

data = data_generation.generateBytes(bitsCount)

floatingPointModulation = afsk_modulation.AfskModulation(data, bitsCount)
afskSignalData = floatingPointModulation.afskModulate()
y = numpy.array(afskSignalData)

fixedPointModulation = afsk_modulation_fixedpoint.AfskModulationFixedPoint(data, bitsCount)
afskSignalDataFixedPoint = fixedPointModulation.afskModulate()
yFixedPoint = numpy.array(afskSignalDataFixedPoint)

x = numpy.linspace(0, len(afskSignalData), len(afskSignalData))

yError = []
for i in range(len(afskSignalData)):
    yError.append(numpy.abs(afskSignalData[i] - afskSignalDataFixedPoint[i]))

minError = numpy.min(yError)
maxError = numpy.max(yError)

print("min error = " + str(minError))
print("max error = " + str(maxError))
print()

errorIdx = yError.index(maxError)
errorBitStartIdx = int(errorIdx / int(definitions_derived.F1200_QUANTS_COUNT_PER_SYMBOL)) * int(definitions_derived.F1200_QUANTS_COUNT_PER_SYMBOL)
startIdx = errorBitStartIdx - int(2 * definitions_derived.F1200_QUANTS_COUNT_PER_SYMBOL)
if startIdx < 0:
    startIdx = 0
    endIdx = errorBitStartIdx + int(5 * definitions_derived.F1200_QUANTS_COUNT_PER_SYMBOL) + 1
else:
    endIdx = errorBitStartIdx + int(3 * definitions_derived.F1200_QUANTS_COUNT_PER_SYMBOL) + 1

plot.plot(x[startIdx:endIdx], y[startIdx:endIdx], color = 'b')
plot.plot(x[startIdx:endIdx], yFixedPoint[startIdx:endIdx], color = 'r')

plot.plot(x[startIdx:endIdx], yError[startIdx:endIdx], color = 'black')

plot.axvline(errorIdx, color = 'r')

for i in range(int(len(x[startIdx:endIdx]) / definitions_derived.F1200_QUANTS_COUNT_PER_SYMBOL) + 1):
    plot.axvline(startIdx + definitions_derived.F1200_QUANTS_COUNT_PER_SYMBOL * (i), color = 'g')

plot.grid(True)
plot.show()
