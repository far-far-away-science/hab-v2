import numpy

import definitions
import definitions_derived

class AfskModulation:
    def __init__(self, data, bitsCount):
        self.data = data
        self.bytesCount = int(bitsCount / 8)
        self.lastByteBitsCount = bitsCount % 8
        self.CONST_QUANT_STEP_SIZE = self.convertToApproximateValue(numpy.float64(definitions.QUANT_STEP_SIZE), 'CONST_QUANT_STEP_SIZE')
        self.CONST_AMPLITUDE_SHIFT = self.convertToApproximateValue(definitions_derived.AMPLITUDE_SHIFT, 'CONST_AMPLITUDE_SHIFT')
        self.CONST_AMPLITUDE_SCALER = self.convertToApproximateValue(definitions_derived.AMPLITUDE_SCALER, 'CONST_AMPLITUDE_SCALER')
        self.CONST_HALF_PERIOD_F1200 = self.convertToApproximateValue(definitions_derived.HALF_PERIOD_F1200, 'CONST_HALF_PERIOD_F1200')
        self.CONST_HALF_PERIOD_F2200 = self.convertToApproximateValue(definitions_derived.HALF_PERIOD_F2200, 'CONST_HALF_PERIOD_F2200')
        self.CONST_ANGULAR_FREQUENCY_F1200 = self.convertToApproximateValue(definitions_derived.ANGULAR_FREQUENCY_F1200, 'CONST_ANGULAR_FREQUENCY_F1200')
        self.CONST_ANGULAR_FREQUENCY_F2200 = self.convertToApproximateValue(definitions_derived.ANGULAR_FREQUENCY_F2200, 'CONST_ANGULAR_FREQUENCY_F2200')
        self.CONST_F1200_QUANTS_COUNT_PER_SYMBOL = self.convertToApproximateValue(definitions_derived.F1200_QUANTS_COUNT_PER_SYMBOL, 'CONST_F1200_QUANTS_COUNT_PER_SYMBOL')
        self.CONST_F2200_QUANTS_COUNT_PER_SYMBOL = self.convertToApproximateValue(definitions_derived.F2200_QUANTS_COUNT_PER_SYMBOL, 'CONST_F2200_QUANTS_COUNT_PER_SYMBOL')

    def advanceBitstreamBit(self, positionByte, positionLastByteBit):
        if positionLastByteBit < 7:
            return (positionByte, positionLastByteBit + 1)
        else:
            return (positionByte + 1, 0)

    def getBit(self, positionByte, positionLastByteBit):
        return (self.data[positionByte] & (1 << positionLastByteBit)) != 0;

    def convertToApproximateValue(self, value, valueName):
        if type(value) != numpy.float64:
            raise RuntimeError("input must be float64")
        return value

    def convertAmplitudeToFinalFormat(self, value):
        return int(value + 0.5)

    def calculateAmplitude(self, isF1200, currentQuant):
        if isF1200:
            angularFrequency = self.CONST_ANGULAR_FREQUENCY_F1200
        else:
            angularFrequency = self.CONST_ANGULAR_FREQUENCY_F2200
        return self.CONST_AMPLITUDE_SHIFT + self.CONST_AMPLITUDE_SCALER * numpy.sin(angularFrequency * currentQuant)

    def cosineGreaterThanOrEqualTo0(self, isF1200, currentQuant):
        if isF1200:
            angularFrequency = self.CONST_ANGULAR_FREQUENCY_F1200
        else:
            angularFrequency = self.CONST_ANGULAR_FREQUENCY_F2200
        param = angularFrequency * currentQuant
        if (param >= numpy.float64(0.0) and param <= numpy.pi / numpy.float64(2.0)) or (param >= numpy.float64(3.0) * numpy.pi / numpy.float64(2.0) and param <= numpy.float64(2.0) * numpy.pi):
            return True
        elif param >= numpy.pi / numpy.float64(2.0) and param <= numpy.float64(3.0) * numpy.pi / numpy.float64(2.0):
            return False
        else:
            raise RuntimeError(str(param) + " is not valid for cosineGreaterThanOrEqualTo0")

    def calculateQuantIdx(self, isSourceF1200, sourceCurrentQuant, isTargetF1200):
        amplitude = self.calculateAmplitude(isSourceF1200, sourceCurrentQuant)
        isSlopePositive = self.cosineGreaterThanOrEqualTo0(isSourceF1200, sourceCurrentQuant)
        if isTargetF1200:
            halfPeriod = self.CONST_HALF_PERIOD_F1200
            angularFrequency = self.CONST_ANGULAR_FREQUENCY_F1200
            quantsCountPerSymbol = self.CONST_F1200_QUANTS_COUNT_PER_SYMBOL
        else:
            halfPeriod = self.CONST_HALF_PERIOD_F2200
            angularFrequency = self.CONST_ANGULAR_FREQUENCY_F2200
            quantsCountPerSymbol = self.CONST_F2200_QUANTS_COUNT_PER_SYMBOL
        if isSlopePositive:
            targetCurrentQuant = numpy.arcsin((amplitude - self.CONST_AMPLITUDE_SHIFT) / self.CONST_AMPLITUDE_SCALER) / angularFrequency
        else:
            targetCurrentQuant = halfPeriod - numpy.arcsin((amplitude - self.CONST_AMPLITUDE_SHIFT) / self.CONST_AMPLITUDE_SCALER) / angularFrequency
        if targetCurrentQuant < numpy.float64(0.0):
            targetCurrentQuant = targetCurrentQuant + quantsCountPerSymbol
        if targetCurrentQuant >= self.CONST_F2200_QUANTS_COUNT_PER_SYMBOL:
            targetCurrentQuant = targetCurrentQuant - quantsCountPerSymbol
        return targetCurrentQuant

    def moveToNextQuantIndex(self, isF1200, currentQuant):
        return currentQuant + self.CONST_QUANT_STEP_SIZE

    def fixedOverflowedCurrentQuantIndex(self, isF1200, currentQuant):
        if isF1200:
            return currentQuant - self.CONST_F1200_QUANTS_COUNT_PER_SYMBOL
        else:
            return currentQuant - self.CONST_F2200_QUANTS_COUNT_PER_SYMBOL

    def afskModulate(self):
        result = []
        abort = False
        positionByte = 0
        positionLastByteBit = 0
        currentF1200Quant = self.convertToApproximateValue(numpy.float64(0.0), 'currentF1200Quant')
        currentF2200Quant = self.convertToApproximateValue(numpy.float64(0.0), 'currentF2200Quant')
        currentFrequencyIsF1200 = True
        currentSymbolQuant = definitions_derived.F1200_QUANTS_COUNT_PER_SYMBOL
        while True:
            if currentSymbolQuant >= definitions_derived.F1200_QUANTS_COUNT_PER_SYMBOL:
                if positionByte >= self.bytesCount and positionLastByteBit >= self.lastByteBitsCount:
                    abort = True
                else:
                    currentSymbolQuant = 0
                    isOne = self.getBit(positionByte, positionLastByteBit)
                    if not isOne and currentFrequencyIsF1200:
                        currentF2200Quant = self.calculateQuantIdx(True, currentF1200Quant, False)
                        currentFrequencyIsF1200 = False
                    elif isOne and not currentFrequencyIsF1200:
                        currentF1200Quant = self.calculateQuantIdx(False, currentF2200Quant, True)
                        currentFrequencyIsF1200 = True
                    (positionByte, positionLastByteBit) = self.advanceBitstreamBit(positionByte, positionLastByteBit)
            if currentFrequencyIsF1200:
                amplitude = self.calculateAmplitude(True, currentF1200Quant)
                currentF1200Quant = self.moveToNextQuantIndex(True, currentF1200Quant)
                if currentF1200Quant >= self.CONST_F1200_QUANTS_COUNT_PER_SYMBOL:
                    currentF1200Quant = self.fixedOverflowedCurrentQuantIndex(True, currentF1200Quant)
                result.append(self.convertAmplitudeToFinalFormat(amplitude))
            else:
                amplitude = self.calculateAmplitude(False, currentF2200Quant)
                currentF2200Quant = self.moveToNextQuantIndex(False, currentF2200Quant)
                if currentF2200Quant >= self.CONST_F2200_QUANTS_COUNT_PER_SYMBOL:
                    currentF2200Quant = self.fixedOverflowedCurrentQuantIndex(False, currentF2200Quant)
                result.append(self.convertAmplitudeToFinalFormat(amplitude))
            if abort:
                break
            currentSymbolQuant += definitions.QUANT_STEP_SIZE
        return result
