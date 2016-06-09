import numpy

import fixedpoint
import trigtables
import definitions
import definitions_derived
import afsk_modulation_fixedpoint

class AfskModulationFixedPointFastDiv(afsk_modulation_fixedpoint.AfskModulationFixedPoint):
    def __init__(self, data, bitsCount):
        afsk_modulation_fixedpoint.AfskModulationFixedPoint.__init__(self, data, bitsCount)
        self.divDataF1200 = {}
        self.divDataF2200 = {}

    def findBestFastDivision(self, divisor, maxValue):
        firstDivisorPowerOfTwo = 0
        while (divisor & 1) == 0:
            firstDivisorPowerOfTwo += 1
            divisor = divisor >> 1
        if firstDivisorPowerOfTwo > 0:
            maxValue = maxValue >> firstDivisorPowerOfTwo
        for i in range(1, 31):
            testLastDivisorPowerOfTwo = i
            testMultiplier = numpy.uint32((1 << testLastDivisorPowerOfTwo) / divisor + 0.5)
            if testMultiplier > 0:
                maxMultipliedValue = numpy.uint64(testMultiplier) * maxValue
                if maxMultipliedValue > numpy.iinfo(numpy.uint32).max:
                    break
                multiplier = testMultiplier
                lastDivisorPowerOfTwo = testLastDivisorPowerOfTwo
        return (firstDivisorPowerOfTwo, multiplier, lastDivisorPowerOfTwo)

    def fastDiv(self, value, firstDivisorPowerOfTwo, multiplier, lastDivisorPowerOfTwo):
        if value > numpy.iinfo(numpy.uint32).max:
            raise RuntimeError(str(value) + ' number is too large')
        r1 = numpy.uint32(value >> firstDivisorPowerOfTwo)
        r2 = numpy.uint64(r1) * multiplier
        if r2 > numpy.iinfo(numpy.uint32).max:
            raise RuntimeError(str(r2) + ' number is too large (' + str(r1) + ' * ' + str(multiplier) + ')')
        return numpy.uint32(numpy.uint32(r2) >> lastDivisorPowerOfTwo)

    def calculateTrigArg(self, isF1200, currentQuant):
        if isF1200:
            result = (self.CONST_TRIG_PARAM_SCALER_F1200 * currentQuant)
            if 'CONST_PRECISION_TRIG_PARAM_DIVISOR' not in self.divDataF1200.keys():
                (trigArg, self.CONST_PRECISION_TRIG_PARAM_ROUND_SUMMAND, self.CONST_PRECISION_TRIG_PARAM_DIVISOR) = \
                    result.convert2Precision(definitions.PRECISION_TRIG_ARG)
                maxValue = \
                    self.CONST_TRIG_PARAM_SCALER_F1200.getInternalRepresentation() * self.CONST_F1200_QUANTS_COUNT_PER_SYMBOL.getInternalRepresentation()
                (firstDivisorPowerOfTwo, multiplier, lastDivisorPowerOfTwo) = \
                    self.findBestFastDivision(self.CONST_PRECISION_TRIG_PARAM_DIVISOR, maxValue + self.CONST_PRECISION_TRIG_PARAM_ROUND_SUMMAND)
                self.divDataF1200['CONST_PRECISION_TRIG_PARAM_DIVISOR'] = (firstDivisorPowerOfTwo, multiplier, lastDivisorPowerOfTwo)
            else:
                (firstDivisorPowerOfTwo, multiplier, lastDivisorPowerOfTwo) = self.divDataF1200['CONST_PRECISION_TRIG_PARAM_DIVISOR']
        else:
            result = (self.CONST_TRIG_PARAM_SCALER_F2200 * currentQuant)
            if 'CONST_PRECISION_TRIG_PARAM_DIVISOR' not in self.divDataF2200.keys():
                (trigArg, self.CONST_PRECISION_TRIG_PARAM_ROUND_SUMMAND, self.CONST_PRECISION_TRIG_PARAM_DIVISOR) = \
                    result.convert2Precision(definitions.PRECISION_TRIG_ARG)
                maxValue = \
                    self.CONST_TRIG_PARAM_SCALER_F2200.getInternalRepresentation() * self.CONST_F2200_QUANTS_COUNT_PER_SYMBOL.getInternalRepresentation()
                (firstDivisorPowerOfTwo, multiplier, lastDivisorPowerOfTwo) = \
                    self.findBestFastDivision(self.CONST_PRECISION_TRIG_PARAM_DIVISOR, maxValue + self.CONST_PRECISION_TRIG_PARAM_ROUND_SUMMAND)
                self.divDataF2200['CONST_PRECISION_TRIG_PARAM_DIVISOR'] = (firstDivisorPowerOfTwo, multiplier, lastDivisorPowerOfTwo)
            else:
                (firstDivisorPowerOfTwo, multiplier, lastDivisorPowerOfTwo) = self.divDataF2200['CONST_PRECISION_TRIG_PARAM_DIVISOR']
        value = self.fastDiv(result.getInternalRepresentation() + self.CONST_PRECISION_TRIG_PARAM_ROUND_SUMMAND, firstDivisorPowerOfTwo, multiplier, lastDivisorPowerOfTwo)
        if value >= len(self.trigTables.sineValues):
            value = len(self.trigTables.sineValues) - 1
        return fixedpoint.FixedPointNumber(value, definitions.PRECISION_TRIG_ARG)

    def calculateQuantIdxFromAmplitude(self, isF1200, reciprocalAngularFrequency, amplitude):
        result = (amplitude * self.CONST_INVERSE_TRIG_SCALER)

        if 'CONST_PRECISION_INVERSE_TRIG_PARAM_DIVISOR' not in self.divDataF1200.keys():
            (inverseTrigArg, self.CONST_PRECISION_INVERSE_TRIG_PARAM_ROUND_SUMMAND, self.CONST_PRECISION_INVERSE_TRIG_PARAM_DIVISOR) = \
                result.convert2Precision(definitions.PRECISION_INVERSE_TRIG_ARG)
            maxValue = max(self.trigTables.sineValues).getInternalRepresentation() * self.CONST_INVERSE_TRIG_SCALER.getInternalRepresentation()
            (firstDivisorPowerOfTwo, multiplier, lastDivisorPowerOfTwo) = \
                self.findBestFastDivision(self.CONST_PRECISION_INVERSE_TRIG_PARAM_DIVISOR, maxValue + self.CONST_PRECISION_INVERSE_TRIG_PARAM_ROUND_SUMMAND)
            self.divDataF1200['CONST_PRECISION_INVERSE_TRIG_PARAM_DIVISOR'] = (firstDivisorPowerOfTwo, multiplier, lastDivisorPowerOfTwo)
        else:
            (firstDivisorPowerOfTwo, multiplier, lastDivisorPowerOfTwo) = self.divDataF1200['CONST_PRECISION_INVERSE_TRIG_PARAM_DIVISOR']

        inverseTrigArg = self.fastDiv(result.getInternalRepresentation() + self.CONST_PRECISION_INVERSE_TRIG_PARAM_ROUND_SUMMAND, firstDivisorPowerOfTwo, multiplier, lastDivisorPowerOfTwo)

        if inverseTrigArg >= len(self.trigTables.arcSineValues):
            inverseTrigArg = len(self.trigTables.arcSineValues) - 1

        result = (self.trigTables.getScaledArcSineValue(inverseTrigArg) * reciprocalAngularFrequency)

        if isF1200:
            if 'CONST_PRECISION_QUANT_DIVISOR' not in self.divDataF1200.keys():
                (inverseTrigArg, self.CONST_PRECISION_QUANT_ROUND_SUMMAND, self.CONST_PRECISION_QUANT_DIVISOR) = \
                    result.convert2Precision(definitions.PRECISION_QUANT)
                maxValue = \
                    max(self.trigTables.arcSineValues).getInternalRepresentation() * self.CONST_RECIPROCAL_ANGULAR_FREQUENCY_F1200.getInternalRepresentation()
                (firstDivisorPowerOfTwo, multiplier, lastDivisorPowerOfTwo) = self.findBestFastDivision(self.CONST_PRECISION_QUANT_DIVISOR, maxValue + self.CONST_PRECISION_QUANT_ROUND_SUMMAND)
                self.divDataF1200['CONST_PRECISION_QUANT_DIVISOR'] = (firstDivisorPowerOfTwo, multiplier, lastDivisorPowerOfTwo)
            else:
                (firstDivisorPowerOfTwo, multiplier, lastDivisorPowerOfTwo) = self.divDataF1200['CONST_PRECISION_QUANT_DIVISOR']
        else:
            if 'CONST_PRECISION_QUANT_DIVISOR' not in self.divDataF2200.keys():
                (inverseTrigArg, self.CONST_PRECISION_QUANT_ROUND_SUMMAND, self.CONST_PRECISION_QUANT_DIVISOR) = \
                    result.convert2Precision(definitions.PRECISION_QUANT)
                maxValue = \
                    max(self.trigTables.arcSineValues).getInternalRepresentation() * self.CONST_RECIPROCAL_ANGULAR_FREQUENCY_F2200.getInternalRepresentation()
                (firstDivisorPowerOfTwo, multiplier, lastDivisorPowerOfTwo) = self.findBestFastDivision(self.CONST_PRECISION_QUANT_DIVISOR, maxValue + self.CONST_PRECISION_QUANT_ROUND_SUMMAND)
                self.divDataF2200['CONST_PRECISION_QUANT_DIVISOR'] = (firstDivisorPowerOfTwo, multiplier, lastDivisorPowerOfTwo)
            else:
                (firstDivisorPowerOfTwo, multiplier, lastDivisorPowerOfTwo) = self.divDataF2200['CONST_PRECISION_QUANT_DIVISOR']

        result = fixedpoint.FixedPointNumber(self.fastDiv(result.getInternalRepresentation() + self.CONST_PRECISION_QUANT_ROUND_SUMMAND, firstDivisorPowerOfTwo, multiplier, lastDivisorPowerOfTwo), \
                                             definitions.PRECISION_QUANT)

        if isF1200:
            if result > self.CONST_F1200_QUANTS_COUNT_PER_SYMBOL:
                result = self.CONST_F1200_QUANTS_COUNT_PER_SYMBOL
        else:
            if result > self.CONST_F2200_QUANTS_COUNT_PER_SYMBOL:
                result = self.CONST_F2200_QUANTS_COUNT_PER_SYMBOL
        return result
