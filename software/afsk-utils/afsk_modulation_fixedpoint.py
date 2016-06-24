import numpy

import fixedpoint
import trigtables
import definitions
import afsk_modulation
import definitions_derived

class AfskModulationFixedPoint(afsk_modulation.AfskModulation):
    def __init__(self, precisionData, data, bitsCount):
        self.precisionData = precisionData
        afsk_modulation.AfskModulation.__init__(self, data, bitsCount)
        self.trigTables = trigtables.TrigTables(precisionData, definitions.TRIG_TABLE_SIZE, definitions.INVERSE_TRIG_TABLE_SIZE)
        self.CONST_TWO_PI = self.precisionData.createFixedPoint(numpy.pi * numpy.float64(2.0) * self.trigTables.CONST_SINE_SCALER, self.precisionData.PRECISION_TRIG_ARG)
        self.CONST_PI_OVER_TWO = self.precisionData.createFixedPoint(numpy.pi / numpy.float64(2.0) * self.trigTables.CONST_SINE_SCALER, self.precisionData.PRECISION_TRIG_ARG)
        self.CONST_THREE_HALFS_PI = self.precisionData.createFixedPoint(numpy.pi * numpy.float64(3.0) / numpy.float64(2.0) * self.trigTables.CONST_SINE_SCALER, self.precisionData.PRECISION_TRIG_ARG)
        self.CONST_AMPLITUDE = self.precisionData.createFixedPoint(definitions.QUANT_MAX_VALUE, self.precisionData.PRECISION_TRIG)
        self.CONST_TRIG_PARAM_SCALER_F1200 = self.precisionData.createFixedPoint(definitions_derived.ANGULAR_FREQUENCY_F1200 * self.trigTables.CONST_SINE_SCALER, self.precisionData.PRECISION_TRIG_SCALER)
        self.CONST_TRIG_PARAM_SCALER_F2200 = self.precisionData.createFixedPoint(definitions_derived.ANGULAR_FREQUENCY_F2200 * self.trigTables.CONST_SINE_SCALER, self.precisionData.PRECISION_TRIG_SCALER)
        self.CONST_INVERSE_TRIG_SCALER = self.precisionData.createFixedPoint(self.trigTables.CONST_ARC_SINE_SCALER / definitions_derived.AMPLITUDE_SCALER, self.precisionData.PRECISION_INVERSE_TRIG_SCALER)
        self.CONST_RECIPROCAL_ANGULAR_FREQUENCY_F1200 = self.precisionData.createFixedPoint(numpy.float64(1.0) / definitions_derived.ANGULAR_FREQUENCY_F1200, self.precisionData.PRECISION_RECIPROCAL_ANGULAR_FREQUENCY)
        self.CONST_RECIPROCAL_ANGULAR_FREQUENCY_F2200 = self.precisionData.createFixedPoint(numpy.float64(1.0) / definitions_derived.ANGULAR_FREQUENCY_F2200, self.precisionData.PRECISION_RECIPROCAL_ANGULAR_FREQUENCY)

    def convertAmplitudeToFinalFormat(self, value):
        (result, self.CONST_PRECISION_OUTPUT_AMPLITUDE_ROUNT_SUMMAND, self.CONST_PRECISION_OUTPUT_AMPLITUDE_DIVISOR) = value.convert2Precision(self.precisionData.PRECISION_AMPLITUDE_OUTPUT)
        return result.getInternalRepresentation()

    def convertToApproximateValue(self, value, valueName):
        if valueName == 'CONST_QUANT_STEP_SIZE' or \
           valueName == 'CONST_F1200_QUANTS_COUNT_PER_SYMBOL' or \
           valueName == 'CONST_F2200_QUANTS_COUNT_PER_SYMBOL' or \
           valueName == 'CONST_HALF_PERIOD_F1200' or \
           valueName == 'CONST_HALF_PERIOD_F2200' or \
           valueName == 'currentF1200Quant' or \
           valueName == 'currentF2200Quant':
            return self.precisionData.createFixedPoint(value, self.precisionData.PRECISION_QUANT)
        elif valueName == 'CONST_AMPLITUDE_SCALER' or \
             valueName == 'CONST_AMPLITUDE_SHIFT':
            return self.precisionData.createFixedPoint(value, self.precisionData.PRECISION_AMPLITUDE)

    def calculateTrigArg(self, isF1200, currentQuant):
        if isF1200:
            (trigArg, self.CONST_PRECISION_TRIG_PARAM_ROUND_SUMMAND, self.CONST_PRECISION_TRIG_PARAM_DIVISOR) = (self.CONST_TRIG_PARAM_SCALER_F1200 * currentQuant).convert2Precision(self.precisionData.PRECISION_TRIG_ARG)
        else:
            (trigArg, self.CONST_PRECISION_TRIG_PARAM_ROUND_SUMMAND, self.CONST_PRECISION_TRIG_PARAM_DIVISOR) = (self.CONST_TRIG_PARAM_SCALER_F2200 * currentQuant).convert2Precision(self.precisionData.PRECISION_TRIG_ARG)
        return trigArg

    def calculateAmplitude(self, isF1200, currentQuant):
        result = self.trigTables.getScaledSineValue(self.calculateTrigArg(isF1200, currentQuant))
        if result > self.CONST_AMPLITUDE:
            raise RuntimeError(str(result) + 'is not a valid result for calculateAmplitude')
        return result

    def cosineGreaterThanOrEqualTo0(self, isF1200, currentQuant):
        trigArg = self.calculateTrigArg(isF1200, currentQuant)
        if (trigArg >= fixedpoint.Zero and trigArg <= self.CONST_PI_OVER_TWO) or \
           (trigArg >= self.CONST_THREE_HALFS_PI and trigArg <= self.CONST_TWO_PI):
            return True
        elif trigArg >= self.CONST_PI_OVER_TWO and trigArg <= self.CONST_THREE_HALFS_PI:
            return False
        else:
            raise RuntimeError(str(trigArg) + " is not valid for cosineGreaterThanOrEqualTo0")

    def calculateQuantIdxFromAmplitude(self, isF1200, reciprocalAngularFrequency, amplitude):
        (inverseTrigArg, self.CONST_PRECISION_INVERSE_TRIG_PARAM_ROUND_SUMMAND, self.CONST_PRECISION_INVERSE_TRIG_PARAM_DIVISOR) = (amplitude * self.CONST_INVERSE_TRIG_SCALER).convert2Precision(self.precisionData.PRECISION_INVERSE_TRIG_ARG)
        (result, self.CONST_PRECISION_QUANT_ROUND_SUMMAND, self.CONST_PRECISION_QUANT_DIVISOR) = (self.trigTables.getScaledArcSineValue(inverseTrigArg) * reciprocalAngularFrequency).convert2Precision(self.precisionData.PRECISION_QUANT)
        return result

    def calculateQuantIdx(self, isSourceF1200, sourceCurrentQuant, isTargetF1200):
        amplitude = self.calculateAmplitude(isSourceF1200, sourceCurrentQuant)
        isSlopePositive = self.cosineGreaterThanOrEqualTo0(isSourceF1200, sourceCurrentQuant)
        if isTargetF1200:
            halfPeriod = self.CONST_HALF_PERIOD_F1200
            reciprocalAngularFrequency = self.CONST_RECIPROCAL_ANGULAR_FREQUENCY_F1200
        else:
            halfPeriod = self.CONST_HALF_PERIOD_F2200
            reciprocalAngularFrequency = self.CONST_RECIPROCAL_ANGULAR_FREQUENCY_F2200
        result = self.calculateQuantIdxFromAmplitude(isTargetF1200, reciprocalAngularFrequency, amplitude)
        if not isSlopePositive:
            if result > halfPeriod:
                if isTargetF1200:
                    quantsCount = self.CONST_F1200_QUANTS_COUNT_PER_SYMBOL
                else:
                    quantsCount = self.CONST_F2200_QUANTS_COUNT_PER_SYMBOL
                result = quantsCount + halfPeriod - result
            else:
                result = halfPeriod - result
        if isTargetF1200:
            if result < fixedpoint.Zero or result > self.CONST_F1200_QUANTS_COUNT_PER_SYMBOL:
                raise RuntimeError(str(result) + 'is not a valid result for calculateQuantIdx')
        else:
            if result < fixedpoint.Zero or result > self.CONST_F2200_QUANTS_COUNT_PER_SYMBOL:
                raise RuntimeError(str(result) + 'is not a valid result for calculateQuantIdx')
        return result
