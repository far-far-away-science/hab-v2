import numpy
import matplotlib.pyplot as plot

import afsk_modulation
import data_generation
import definitions_derived
import afsk_modulation_fixedpoint_fast_div

def filterData(errorData, startIdx, endIdx, numberOfSymbolsAroundMaxError):
    errorData = errorData[int(startIdx) : int(endIdx)]
    errorIdx = errorData.index(numpy.max(errorData))
    errorBitStartIdx = int(errorIdx / int(definitions_derived.F1200_QUANTS_COUNT_PER_SYMBOL)) * int(definitions_derived.F1200_QUANTS_COUNT_PER_SYMBOL)
    startIdx = errorBitStartIdx - int(numberOfSymbolsAroundMaxError * definitions_derived.F1200_QUANTS_COUNT_PER_SYMBOL)
    if startIdx < 0:
        startIdx = 0
        endIdx = errorBitStartIdx + int((numberOfSymbolsAroundMaxError + 1) * definitions_derived.F1200_QUANTS_COUNT_PER_SYMBOL) + 1
    else:
        endIdx = errorBitStartIdx + int((numberOfSymbolsAroundMaxError + 1) * definitions_derived.F1200_QUANTS_COUNT_PER_SYMBOL) + 1
    return (startIdx, endIdx, errorIdx)

bitsCount = int(128 * 8 + 128 / 5 * 8 + 1)

data = data_generation.generateBytes(bitsCount)

floatingPointModulation = afsk_modulation.AfskModulation(data, bitsCount)
afskSignalData = floatingPointModulation.afskModulate()
y = numpy.array(afskSignalData)

fixedPointModulation = afsk_modulation_fixedpoint_fast_div.AfskModulationFixedPointFastDiv(data, bitsCount)
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

(startIdx, endIdx, errorIdx) = filterData(yError, 0, len(yError), 2)

plot.plot(x[startIdx:endIdx], y[startIdx:endIdx], color = 'b')
plot.plot(x[startIdx:endIdx], yFixedPoint[startIdx:endIdx], color = 'r')

plot.plot(x[startIdx:endIdx], yError[startIdx:endIdx], color = 'black')

plot.axvline(errorIdx, color = 'r')

for i in range(int(len(x[startIdx:endIdx]) / definitions_derived.F1200_QUANTS_COUNT_PER_SYMBOL) + 1):
    plot.axvline(startIdx + definitions_derived.F1200_QUANTS_COUNT_PER_SYMBOL * (i), color = 'g')

plot.grid(True)
plot.show()
