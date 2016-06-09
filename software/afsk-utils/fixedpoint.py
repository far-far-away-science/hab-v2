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
            raise RuntimeError('increasing precision is not supported')

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

    def __str__(self):
        scaler = numpy.uint32(10 ** self.precision)
        whole = numpy.uint32(self.value / scaler)
        fractional = self.value - whole * scaler
        if self.precision != 0:
            return str(whole) + '.' + ('{:0>' + str(self.precision) + '}').format(str(fractional)) + ' {v=' + str(self.value) + ',p=' + str(self.precision) + '}'
        else:
            return str(whole) + ' {v=' + str(self.value) + ',p=' + str(self.precision) + '}'

    def __repr__(self):
        return str(self)

Zero = FixedPointNumber(0, 0)
