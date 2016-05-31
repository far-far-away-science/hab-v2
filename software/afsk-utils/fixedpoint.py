import numpy

class FixedPointNumber:
    def __init__(self, value, precision):
        if type(value) == numpy.uint32:
            self.value = value
        else:
            self.value = numpy.uint32(value * numpy.float64(10 ** precision) + numpy.float64(0.5))
        self.precision = numpy.uint8(precision)

    def convert2Precision(self, precision):
        if precision == self.precision:
            return (FixedPointNumber(self.value, self.precision), numpy.uint32(0), numpy.uint32(1))
        elif precision < self.precision:
            scaler = numpy.uint32(10 ** (self.precision - precision))
            summand = numpy.uint32(scaler / 2)
            return (FixedPointNumber(numpy.uint32((self.value + summand) / scaler), precision), summand, scaler)
        else:
            scaler = numpy.uint32(10 ** (precision * self.precision))
            newValue = self.value * scaler
            if result > numpy.iinfo(numpy.uin32).max:
                raise OverflowError("unable to convert 2 precision " + str(precision) + " as it results in overflow")
            return (FixedPointNumber(numpy.uint32(newValue), precision), 0, scaler)

    def getInternalRepresentation(self):
        return self.value

    def ensureSamePrecision(self, other):
        if type(other) != FixedPointNumber:
            raise RuntimeError("operation supports only fixed point number as other argument")
        if self.precision != other.precision and self.value != 0 and other.value != 0:
            raise RuntimeError("operation supports only fixed point numbers with the same precision")

    def __eq__(self, other):
        self.ensureSamePrecision(other)
        return self.value == other.value

    def __ne__(self, other):
        return self.__eq__(other)

    def __lt__(self, other):
        self.ensureSamePrecision(other)
        return self.value < other.value

    def __le__(self, other):
        self.ensureSamePrecision(other)
        return self.value <= other.value

    def __gt__(self, other):
        self.ensureSamePrecision(other)
        return self.value > other.value

    def __ge__(self, other):
        self.ensureSamePrecision(other)
        return self.value >= other.value

    def __mul__(self, other):
        result = numpy.float64(self.value) * other.value
        if result > numpy.iinfo(numpy.uint32).max:
            raise OverflowError("overflow multiplying 2 numbers " + str(self.value) + " * " + str(other.value))
        return FixedPointNumber(numpy.uint32(result), self.precision + other.precision)

    def __add__(self, other):
        self.ensureSamePrecision(other)
        result = self.value + other.value
        if result > numpy.iinfo(numpy.uint32).max:
            raise OverflowError("overflow adding 2 numbers " + str(self.value) + " + " + str(other.value))
        return FixedPointNumber(numpy.uint32(result), self.precision)

    def __sub__(self, other):
        self.ensureSamePrecision(other)
        if self.value < other.value:
            raise OverflowError("fixed point number supports only positive numbers. attempted " + str(self.value) + " - " + str(other.value))
        return FixedPointNumber(numpy.uint32(self.value - other.value), self.precision)

Zero = FixedPointNumber(0, 0)
