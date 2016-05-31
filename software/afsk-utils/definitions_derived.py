import math
import numpy
import definitions

AMPLITUDE_SCALER = (numpy.float64(definitions.QUANT_MAX_VALUE) - numpy.float64(definitions.QUANT_MIN_VALUE)) / numpy.float64(2.0)
AMPLITUDE_SHIFT  = numpy.float64(AMPLITUDE_SCALER) + numpy.float64(definitions.QUANT_MIN_VALUE)

TRIG_MULTIPLIER         = int(definitions.TRIG_TABLE_SIZE / (numpy.float64(2.0) * numpy.float64(numpy.pi)) + numpy.float64(0.5))
INVERSE_TRIG_MULTIPLIER = int(definitions.INVERSE_TRIG_TABLE_SIZE / numpy.float64(2.0) + numpy.float64(0.5))

# should be around 5-10 ms for HX-1 warm-up
LEADING_WARMUP_QUANTS_COUNT = numpy.float64(definitions.APRS_SIGNAL_GENERATION_FREQUENCY) * numpy.float64(0.01)

F1200_QUANTS_COUNT_PER_SYMBOL = numpy.float64(definitions.APRS_SIGNAL_GENERATION_FREQUENCY) / numpy.float64(1200.0)
F2200_QUANTS_COUNT_PER_SYMBOL = numpy.float64(definitions.APRS_SIGNAL_GENERATION_FREQUENCY) / numpy.float64(2200.0)

AMPLITUDE_SCALER            = (numpy.float64(definitions.QUANT_MAX_VALUE) - numpy.float64(definitions.QUANT_MIN_VALUE)) / numpy.float64(2.0)
RECIPROCAL_AMPLITUDE_SCALER = numpy.float64(1.0) / numpy.float64(AMPLITUDE_SCALER)

AMPLITUDE_SHIFT = numpy.float64(AMPLITUDE_SCALER) + numpy.float64(definitions.QUANT_MIN_VALUE)

HALF_PERIOD_F1200 = numpy.float64(F1200_QUANTS_COUNT_PER_SYMBOL) / numpy.float64(2.0)
HALF_PERIOD_F2200 = numpy.float64(F2200_QUANTS_COUNT_PER_SYMBOL) / numpy.float64(2.0)

ANGULAR_FREQUENCY_F1200 = numpy.float64(2.0) * numpy.float64(numpy.pi) / numpy.float64(F1200_QUANTS_COUNT_PER_SYMBOL)
ANGULAR_FREQUENCY_F2200 = numpy.float64(2.0) * numpy.float64(numpy.pi) / numpy.float64(F2200_QUANTS_COUNT_PER_SYMBOL)

RECIPROCAL_ANGULAR_FREQUENCY_F1200 = numpy.float64(1.0) / numpy.float64(ANGULAR_FREQUENCY_F1200)
RECIPROCAL_ANGULAR_FREQUENCY_F2200 = numpy.float64(1.0) / numpy.float64(ANGULAR_FREQUENCY_F2200)

LEADING_WARMUP_QUANTS_COUNT = int(definitions.APRS_SIGNAL_GENERATION_FREQUENCY * definitions.TRANSMITTER_WARMUP_TIME_MILLISECONDS / numpy.float64(1000.0))
