import numpy
import fixedpoint
import trigtables
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
                currentF1200Quant = currentF1200Quant + self.CONST_QUANT_STEP_SIZE
                if currentF1200Quant >= self.CONST_F1200_QUANTS_COUNT_PER_SYMBOL:
                    currentF1200Quant = currentF1200Quant - self.CONST_F1200_QUANTS_COUNT_PER_SYMBOL
                result.append(self.convertAmplitudeToFinalFormat(amplitude))
            else:
                amplitude = self.calculateAmplitude(False, currentF2200Quant)
                currentF2200Quant = currentF2200Quant + self.CONST_QUANT_STEP_SIZE
                if currentF2200Quant >= self.CONST_F2200_QUANTS_COUNT_PER_SYMBOL:
                    currentF2200Quant = currentF2200Quant - self.CONST_F2200_QUANTS_COUNT_PER_SYMBOL
                result.append(self.convertAmplitudeToFinalFormat(amplitude))
            if abort:
                break
            currentSymbolQuant += definitions.QUANT_STEP_SIZE
        return result

class AfskModulationFixedPoint(AfskModulation):
    def __init__(self, data, bitsCount):
        AfskModulation.__init__(self, data, bitsCount)
        self.trigTables = trigtables.TrigTables(definitions.TRIG_TABLE_SIZE, definitions.INVERSE_TRIG_TABLE_SIZE)
        self.CONST_TWO_PI = fixedpoint.FixedPointNumber(numpy.pi * numpy.float64(2.0) * self.trigTables.CONST_SINE_SCALER, definitions.PRECISION_TRIG_ARG)
        self.CONST_PI_OVER_TWO = fixedpoint.FixedPointNumber(numpy.pi / numpy.float64(2.0) * self.trigTables.CONST_SINE_SCALER, definitions.PRECISION_TRIG_ARG)
        self.CONST_THREE_HALFS_PI = fixedpoint.FixedPointNumber(numpy.pi * numpy.float64(3.0) / numpy.float64(2.0) * self.trigTables.CONST_SINE_SCALER, definitions.PRECISION_TRIG_ARG)
        self.CONST_AMPLITUDE = fixedpoint.FixedPointNumber(definitions.QUANT_MAX_VALUE, definitions.PRECISION_TRIG)
        self.CONST_TRIG_PARAM_SCALER_F1200 = fixedpoint.FixedPointNumber(definitions_derived.ANGULAR_FREQUENCY_F1200 * self.trigTables.CONST_SINE_SCALER, definitions.PRECISION_TRIG_SCALER)
        self.CONST_TRIG_PARAM_SCALER_F2200 = fixedpoint.FixedPointNumber(definitions_derived.ANGULAR_FREQUENCY_F2200 * self.trigTables.CONST_SINE_SCALER, definitions.PRECISION_TRIG_SCALER)
        self.CONST_INVERSE_TRIG_SCALER = fixedpoint.FixedPointNumber(self.trigTables.CONST_ARC_SINE_SCALER / definitions_derived.AMPLITUDE_SCALER, definitions.PRECISION_INVERSE_TRIG_SCALER)
        self.CONST_RECIPROCAL_ANGULAR_FREQUENCY_F1200 = fixedpoint.FixedPointNumber(numpy.float64(1.0) / definitions_derived.ANGULAR_FREQUENCY_F1200, definitions.PRECISION_RECIPROCAL_ANGULAR_FREQUENCY)
        self.CONST_RECIPROCAL_ANGULAR_FREQUENCY_F2200 = fixedpoint.FixedPointNumber(numpy.float64(1.0) / definitions_derived.ANGULAR_FREQUENCY_F2200, definitions.PRECISION_RECIPROCAL_ANGULAR_FREQUENCY)

    def convertAmplitudeToFinalFormat(self, value):
        (result, self.CONST_PRECISION_OUTPUT_AMPLITUDE_ROUNT_SUMMAND, self.CONST_PRECISION_OUTPUT_AMPLITUDE_DIVISOR) = value.convert2Precision(definitions.PRECISION_AMPLITUDE_OUTPUT)
        return result.getInternalRepresentation()

    def convertToApproximateValue(self, value, valueName):
        if valueName == 'CONST_QUANT_STEP_SIZE' or \
           valueName == 'CONST_F1200_QUANTS_COUNT_PER_SYMBOL' or \
           valueName == 'CONST_F2200_QUANTS_COUNT_PER_SYMBOL' or \
           valueName == 'CONST_HALF_PERIOD_F1200' or \
           valueName == 'CONST_HALF_PERIOD_F2200' or \
           valueName == 'currentF1200Quant' or \
           valueName == 'currentF2200Quant':
            return fixedpoint.FixedPointNumber(value, definitions.PRECISION_QUANT)
        elif valueName == 'CONST_AMPLITUDE_SCALER' or \
             valueName == 'CONST_AMPLITUDE_SHIFT':
            return fixedpoint.FixedPointNumber(value, definitions.PRECISION_AMPLITUDE)

    def calculateAmplitude(self, isF1200, currentQuant):
        if isF1200:
            (trigArg, self.CONST_PRECISION_TRIG_PARAM_ROUND_SUMMAND, self.CONST_PRECISION_TRIG_PARAM_DIVISOR) = (self.CONST_TRIG_PARAM_SCALER_F1200 * currentQuant).convert2Precision(definitions.PRECISION_TRIG_ARG)
        else:
            (trigArg, self.CONST_PRECISION_TRIG_PARAM_ROUND_SUMMAND, self.CONST_PRECISION_TRIG_PARAM_DIVISOR) = (self.CONST_TRIG_PARAM_SCALER_F2200 * currentQuant).convert2Precision(definitions.PRECISION_TRIG_ARG)
        result = self.trigTables.getScaledSineValue(trigArg)
        if result > self.CONST_AMPLITUDE:
            raise RuntimeError(str(result) + 'is not a valid result for calculateAmplitude')
        return result

    def cosineGreaterThanOrEqualTo0(self, isF1200, currentQuant):
        if isF1200:
            (trigArg, self.CONST_PRECISION_TRIG_PARAM_ROUND_SUMMAND, self.CONST_PRECISION_TRIG_PARAM_DIVISOR) = (self.CONST_TRIG_PARAM_SCALER_F1200 * currentQuant).convert2Precision(definitions.PRECISION_TRIG_ARG)
        else:
            (trigArg, self.CONST_PRECISION_TRIG_PARAM_ROUND_SUMMAND, self.CONST_PRECISION_TRIG_PARAM_DIVISOR) = (self.CONST_TRIG_PARAM_SCALER_F2200 * currentQuant).convert2Precision(definitions.PRECISION_TRIG_ARG)
        if (trigArg >= fixedpoint.Zero and trigArg <= self.CONST_PI_OVER_TWO) or \
           (trigArg >= self.CONST_THREE_HALFS_PI and trigArg <= self.CONST_TWO_PI):
            return True
        elif trigArg >= self.CONST_PI_OVER_TWO and trigArg <= self.CONST_THREE_HALFS_PI:
            return False
        else:
            raise RuntimeError(str(trigArg) + " is not valid for cosineGreaterThanOrEqualTo0")

    def calculateQuantIdx(self, isSourceF1200, sourceCurrentQuant, isTargetF1200):
        amplitude = self.calculateAmplitude(isSourceF1200, sourceCurrentQuant)
        isSlopePositive = self.cosineGreaterThanOrEqualTo0(isSourceF1200, sourceCurrentQuant)
        if isTargetF1200:
            halfPeriod = self.CONST_HALF_PERIOD_F1200
            reciprocalAngularFrequency = self.CONST_RECIPROCAL_ANGULAR_FREQUENCY_F1200
        else:
            halfPeriod = self.CONST_HALF_PERIOD_F2200
            reciprocalAngularFrequency = self.CONST_RECIPROCAL_ANGULAR_FREQUENCY_F2200

        (trigArg, self.CONST_PRECISION_INVERSE_TRIG_PARAM_ROUND_SUMMAND, self.CONST_PRECISION_INVERSE_TRIG_PARAM_DIVISOR) = (amplitude * self.CONST_INVERSE_TRIG_SCALER).convert2Precision(definitions.PRECISION_INVERSE_TRIG_ARG)
        (result, self.CONST_PRECISION_QUANT_ROUND_SUMMAND, self.CONST_PRECISION_QUANT_DIVISOR) = (self.trigTables.getScaledArcSineValue(trigArg) * reciprocalAngularFrequency).convert2Precision(definitions.PRECISION_QUANT)
        if not isSlopePositive:
            if result > halfPeriod:
                if isTargetF1200:
                    quantsCount = self.CONST_F1200_QUANTS_COUNT_PER_SYMBOL
                else:
                    quantsCount = self.CONST_F2200_QUANTS_COUNT_PER_SYMBOL
                result = quantsCount + halfPeriod - result
            else:
                result = halfPeriod - result
        if result < fixedpoint.Zero or result > self.CONST_F1200_QUANTS_COUNT_PER_SYMBOL:
            raise RuntimeError(str(result) + 'is not a valid result for calculateQuantIdx')
        return result
