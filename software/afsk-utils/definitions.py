import numpy

import fixedpoint

AFSK_FREQUENCY = int(1200)

APRS_SIGNAL_GENERATION_FREQUENCY = 80 * AFSK_FREQUENCY # 96000 Hz

QUANT_STEP_SIZE = 1 # APRS_SIGNAL_GENERATION_FREQUENCY must be integer multiple of AFSK_FREQUENCY

TRANSMITTER_WARMUP_TIME_MILLISECONDS = 10

QUANT_MIN_VALUE         = 0
QUANT_MAX_VALUE         = (1 << 12) - 1 # 12 bit dac

TRIG_TABLE_SIZE         = 3072
INVERSE_TRIG_TABLE_SIZE = 3072

class FixedPointFastDivPrecisionData:
    def __init__(self):
        # higher precision reduces precision of approximated fast division as multiplier factor
        # is limited by 32 bit result value

        # 2 precisions below are liked and there will be an overflow if sum is > than 6
        self.PRECISION_QUANT                        = 4
        self.PRECISION_TRIG_SCALER                  = 2

        # 2 precisions below are linked and there will be an overflow if sum is > than 7
        self.PRECISION_INVERSE_TRIG                 = 3
        self.PRECISION_RECIPROCAL_ANGULAR_FREQUENCY = 4

        # 2 precisions below are linked together and there will be an overflow if sum is > 6
        self.PRECISION_AMPLITUDE                    = 3
        self.PRECISION_TRIG                         = 3 # note that this precision is linked to the one above and to the one below!!!
        self.PRECISION_INVERSE_TRIG_SCALER          = 3
        # 2 precisions above are linked together and there will be an overflow if sum is > 6

        self.PRECISION_AMPLITUDE_OUTPUT             = 0
        self.PRECISION_TRIG_ARG                     = 0
        self.PRECISION_INVERSE_TRIG_ARG             = 0

    def createFixedPoint(self, value, precision):
        return fixedpoint.FixedPointNumber(value, precision, 10)

class FixedPointBase2FastDivPrecisionData:
    def __init__(self):
        # higher precision reduces precision of approximated fast division as multiplier factor
        # is limited by 32 bit result value

        # 2 precisions below are liked and there will be an overflow if sum is > than 20
        self.PRECISION_QUANT                        = 14
        self.PRECISION_TRIG_SCALER                  = 6

        # 2 precisions below are linked and there will be an overflow if sum is > than 25
        self.PRECISION_INVERSE_TRIG                 = 9
        self.PRECISION_RECIPROCAL_ANGULAR_FREQUENCY = 16

        # 2 precisions below are linked together and there will be an overflow if sum is > 20
        self.PRECISION_AMPLITUDE                    = 11
        self.PRECISION_TRIG                         = 9 # note that this precision is linked to the one above and to the one below!!!
        self.PRECISION_INVERSE_TRIG_SCALER          = 11
        # 2 precisions above are linked together and there will be an overflow if sum is > 20

        self.PRECISION_AMPLITUDE_OUTPUT             = 0
        self.PRECISION_TRIG_ARG                     = 0
        self.PRECISION_INVERSE_TRIG_ARG             = 0

    def createFixedPoint(self, value, precision):
        return fixedpoint.FixedPointNumber(value, precision, 2)
