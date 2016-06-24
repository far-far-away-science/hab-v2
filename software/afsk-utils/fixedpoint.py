import numpy

class FixedPointNumber:
    def __init__(self, value, precision, base):
        self.base = base
        if type(value) == numpy.uint32:
            self.value = value
        else:
            if value > numpy.iinfo(numpy.uint32).max:
                raise OverflowError("overflow creating fixed point number (value = " + str(value) + ", precision" + str(precision) + ")")
            result = numpy.float64(value) * (self.base ** precision)
            if result > numpy.iinfo(numpy.uint32).max:
                raise OverflowError("overflow creating fixed point number (value = " + str(value) + ", precision" + str(precision) + ")")
            self.value = numpy.uint32(result)
        self.precision = numpy.uint8(precision)

    def convert2Precision(self, precision):
        if precision == self.precision:
            return (FixedPointNumber(self.value, self.precision, self.base), numpy.uint32(0), numpy.uint32(1))
        elif precision < self.precision:
            scaler = numpy.uint32(2 ** (self.precision - precision))
            summand = numpy.uint32(scaler / 2)
            return (FixedPointNumber(numpy.uint32((self.value + summand) / scaler), precision, self.base), summand, scaler)
        else:
            raise RuntimeError('increasing precision is not supported')

    def __str__(self):
        scaler = numpy.uint32(self.base ** self.precision)
        whole = numpy.uint32(self.value / scaler)
        fractional = self.value - whole * scaler
        if self.precision != 0:
            if self.base == 10:
                return str(whole) + '.' + ('{:0>' + str(self.precision) + '}').format(str(fractional)) + ' {v=' + str(self.value) + ',p=' + str(self.precision) + ', b=' + str(self.base) + '}'
            else:
                return str(self.value / scaler) + ' {v=' + str(self.value) + ',p=' + str(self.precision) + ', b=' + str(self.base) + '}'
        else:
            return str(whole) + ' {v=' + str(self.value) + ',p=' + str(self.precision) + ', b=' + str(self.base) + '}'

    def getInternalRepresentation(self):
        return self.value

    def ensureSamePrecision(self, other):
        if type(other) != FixedPointNumber:
            raise RuntimeError("operation supports only fixed point number as other argument")
        if self.precision != other.precision and self.value != 0 and other.value != 0:
            raise RuntimeError("operation supports only fixed point numbers with the same precision")
        if self.base != other.base and self.value != 0 and other.value != 0:
            raise RuntimeError("can only operate on fixed point numbers of the same base")

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
        if self.base != other.base:
            raise RuntimeError("can only operate on fixed point numbers of the same base")
        result = numpy.float64(self.value) * other.value
        if result > numpy.iinfo(numpy.uint32).max:
            raise OverflowError("overflow multiplying 2 numbers " + str(self.value) + " * " + str(other.value))
        return FixedPointNumber(numpy.uint32(result), self.precision + other.precision, self.base)

    def __add__(self, other):
        self.ensureSamePrecision(other)
        result = self.value + other.value
        if result > numpy.iinfo(numpy.uint32).max:
            raise OverflowError("overflow adding 2 numbers " + str(self.value) + " + " + str(other.value))
        return FixedPointNumber(numpy.uint32(result), self.precision, self.base)

    def __sub__(self, other):
        self.ensureSamePrecision(other)
        if self.value < other.value:
            raise OverflowError("fixed point number supports only positive numbers. attempted " + str(self.value) + " - " + str(other.value))
        return FixedPointNumber(numpy.uint32(self.value - other.value), self.precision, self.base)

    def __repr__(self):
        return str(self)

Zero = FixedPointNumber(0, 0, 10)
