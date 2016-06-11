import numpy
import random
import matplotlib.pyplot as plot

import afsk_modulation
import data_generation
import afsk_modulation_fixedpoint_fast_div

bitsCount = int(128 * 8 + 128 / 5 * 8 + 1)

errors = []
bestMaxError = float('+inf')
worstMaxError = float('-inf')

ITERATIONS_COUNT = 500

for i in range(ITERATIONS_COUNT):
    data = data_generation.generateBytes(bitsCount)

    floatingPointModulation = afsk_modulation.AfskModulation(data, bitsCount)
    afskSignalData = floatingPointModulation.afskModulate()

    x = numpy.linspace(1, len(afskSignalData), len(afskSignalData))
    y = numpy.array(afskSignalData)

    fixedPointModulation = afsk_modulation_fixedpoint_fast_div.AfskModulationFixedPointFastDiv(data, bitsCount)
    afskSignalDataFixedPoint = fixedPointModulation.afskModulate()
    yFixedPoint = numpy.array(afskSignalDataFixedPoint)

    yError = []
    for j in range(len(afskSignalData)):
        yError.append(numpy.abs(afskSignalData[j] - afskSignalDataFixedPoint[j]))

    maxError = numpy.max(yError)

    errors.append(maxError)

    if maxError < bestMaxError:
        bestMaxError = maxError
        data_generation.saveBytes(data, "data-best-max-{:0>4}.txt".format(bestMaxError))

    if maxError > worstMaxError:
        worstMaxError = maxError
        data_generation.saveBytes(data, "data-worst-max-{:0>4}.txt".format(worstMaxError))

    print("iteration " + str(i) + " out of " + str(ITERATIONS_COUNT) + ", max error = " + str(maxError))

data_generation.saveBytes(errors, "errors.txt")

print("best max error = " + str(bestMaxError))
print("worst max error = " + str(worstMaxError))
print()

plot.hist(errors, 40, (0, 4100), label = "best max error = " + str(bestMaxError) + " worst max error = " + str(worstMaxError))
plot.grid(True)
plot.show()
