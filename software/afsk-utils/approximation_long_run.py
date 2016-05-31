import numpy

import afsk_modulation
import data_generation
import afsk_modulation_fixedpoint

bitsCount = int(128 * 8 + 128 / 5 * 8 + 1)

worstMinError = float('-inf')
worstMaxError = float('-inf')

for i in range(200):
    data = data_generation.generateBytes(bitsCount)

    floatingPointModulation = afsk_modulation.AfskModulation(data, bitsCount)
    afskSignalData = floatingPointModulation.afskModulate()

    x = numpy.linspace(1, len(afskSignalData), len(afskSignalData))
    y = numpy.array(afskSignalData)

    fixedPointModulation = afsk_modulation_fixedpoint.AfskModulationFixedPoint(data, bitsCount)
    afskSignalDataFixedPoint = fixedPointModulation.afskModulate()
    yFixedPoint = numpy.array(afskSignalDataFixedPoint)

    yError = []
    for i in range(len(afskSignalData)):
        yError.append(numpy.abs(afskSignalData[i] - afskSignalDataFixedPoint[i]))

    minError = numpy.min(yError)
    maxError = numpy.max(yError)

    if minError > worstMinError:
        worstMinError = minError
        data_generation.saveBytes(data, "data-worst-min-" + str(worstMinError) + ".txt")

    if maxError > worstMaxError:
        worstMaxError = maxError
        data_generation.saveBytes(data, "data-worst-max-" + str(worstMaxError) + ".txt")

    print("min error = " + str(minError))
    print("max error = " + str(maxError))
    print()

print("worst min error = " + str(worstMinError))
print("worst max error = " + str(worstMaxError))
print()
