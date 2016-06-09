import numpy

import fixedpoint
import definitions
import definitions_derived

class TrigTables:
    def __init__(self, sineTableSize, arcSineTableSize):
        self.CONST_SINE_SCALER     = sineTableSize / (numpy.float64(2.0) * numpy.pi)
        self.CONST_ARC_SINE_SCALER = arcSineTableSize / numpy.float64(2.0)
        self.sineValues = self.generateSineValues(sineTableSize, self.CONST_SINE_SCALER)
        self.arcSineValues = self.generateArcSineValues(arcSineTableSize, self.CONST_ARC_SINE_SCALER)

    def generateSineValues(self, tableSize, scaler):
        result = []
        for i in range(tableSize + 1):
            sin = definitions_derived.AMPLITUDE_SHIFT + definitions_derived.AMPLITUDE_SCALER * numpy.sin(numpy.float64(i) / scaler)
            value = fixedpoint.FixedPointNumber(sin, definitions.PRECISION_TRIG)
            result.append(value)
        return result

    def generateArcSineValues(self, tableSize, scaler):
        result = []
        for i in range(tableSize + 1):
            arcsin = numpy.arcsin(numpy.float64(2.0) * i / tableSize - numpy.float64(1.0))
            if arcsin >= 0:
                value = fixedpoint.FixedPointNumber(arcsin, definitions.PRECISION_INVERSE_TRIG)
            else:
                value = fixedpoint.FixedPointNumber(arcsin + numpy.pi * numpy.float64(2.0), definitions.PRECISION_INVERSE_TRIG)
            result.append(value)
        return result

    def getScaledSineValue(self, idx):
        if type(idx) == fixedpoint.FixedPointNumber:
            idx = idx.getInternalRepresentation()
        if idx < 0 or idx >= len(self.sineValues):
            raise RuntimeError(str(idx) + ' index is out of range [' + str(0) + ',' + str(len(self.sineValues)) + ']')
        return self.sineValues[idx]

    def getScaledArcSineValue(self, idx):
        if type(idx) == fixedpoint.FixedPointNumber:
            idx = idx.getInternalRepresentation()
        if idx < 0 or idx >= len(self.arcSineValues):
            raise RuntimeError(str(idx) + ' index is out of range [' + str(0) + ',' + str(len(self.arcSineValues)) + ']')
        return self.arcSineValues[idx]
