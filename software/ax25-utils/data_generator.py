import numpy

def invert(data, numberOfBits):
    return (~numpy.uint16(data)) & ((1 << (numberOfBits)) - 1)

def calculateNewAx25DataFromOld(oldNumberOfOnes, byte2Encode):
    (newDataGiventLastBitWasZero, newDataNumberOfBitsA, newNumberOfOnesA, newLastBitGiventLastBitWasZero) = calculateNewAx25DataFromOldImpl(0, oldNumberOfOnes, byte2Encode)
    (newDataGiventLastBitWasOne, newDataNumberOfBitsB, newNumberOfOnesB, newLastBitGiventLastBitWasOne) = calculateNewAx25DataFromOldImpl(1, oldNumberOfOnes, byte2Encode)
    if newDataNumberOfBitsA != newDataNumberOfBitsB:
        raise RuntimeError("number of bits doesn't match")
    if newNumberOfOnesA != newNumberOfOnesB:
        raise RuntimeError("number of bits doesn't match")
    if newDataGiventLastBitWasZero != invert(newDataGiventLastBitWasOne, newDataNumberOfBitsA):
        raise RuntimeError("invert of result givent that last bit was one doesn't equal to result given that last bit was zero")
    if newLastBitGiventLastBitWasZero == newLastBitGiventLastBitWasOne:
        raise RuntimeError("last bit should be different for both cases")
    return (oldNumberOfOnes, byte2Encode, newDataGiventLastBitWasZero, newLastBitGiventLastBitWasZero, newDataGiventLastBitWasOne, newLastBitGiventLastBitWasOne, newDataNumberOfBitsA, newNumberOfOnesA)

def calculateNewAx25DataFromOldImpl(oldLastBit, oldNumberOfOnes, byte2Encode, performStuffing = True):
    newLastBit = oldLastBit
    newDataNumberOfBits = 0
    newNumberOfOnes = oldNumberOfOnes
    newData = numpy.uint16(0)
    # need to insert zero after 5 ones (so we never get 0x7E as it marks start and end of the frame and has 6 ones)
    if performStuffing and newNumberOfOnes == 5:
        if newLastBit == 1: # we can add zero as is as last bit was one
            newLastBit = 0
            newData &= ~(1 << (newDataNumberOfBits))
        else: # last bit was zero so need to invert zero
            newLastBit = 1
            newData |= 1 << (newDataNumberOfBits)
        newNumberOfOnes = 0
        newDataNumberOfBits += 1
    for iBit in range(8):
        bit = byte2Encode & (1 << iBit)
        if bit != 0: # insert one bit
            if newLastBit == 1: # no inversion needed as last bit was one
                newData |= 1 << (newDataNumberOfBits)
            else: # need to invert one to zero as last bit was zero
                newData &= ~(1 << (newDataNumberOfBits))
            newNumberOfOnes += 1
            newDataNumberOfBits += 1
            # need to insert zero after 5 ones (so we never get 0x7E as it marks start and end of the frame and has 6 ones)
            if performStuffing and newNumberOfOnes == 5:
                if newLastBit == 1: # we can add zero as is as last bit was one
                    newLastBit = 0
                    newData &= ~(1 << (newDataNumberOfBits))
                else: # last bit was zero so need to invert zero
                    newLastBit = 1
                    newData |= 1 << (newDataNumberOfBits)
                newNumberOfOnes = 0
                newDataNumberOfBits += 1
        else: # insert 0 bit
            if newLastBit == 1: # we can add zero as is as last bit was one
                newLastBit = 0
                newData &= ~(1 << (newDataNumberOfBits))
            else: # last bit was zero so need to invert zero
                newLastBit = 1
                newData |= 1 << (newDataNumberOfBits)
            newNumberOfOnes = 0
            newDataNumberOfBits += 1
    return (newData, newDataNumberOfBits, newNumberOfOnes, newLastBit)

def getNumberOfBits(bit, data, bitsCount):
    numberOfZeroes = 0
    for i in range(bitsCount):
        if (bit == 0 and ((1 << i) & data) == 0) or \
           (bit == 1 and ((1 << i) & data) != 0):
            numberOfZeroes += 1
    return numberOfZeroes

def generateAllByteToAx25DataCombinations():
    maxOnes = 0
    maxZeroes = 0
    combinations = []
    totalNumberOfOnes = 0
    totalNumberOfZeroes = 0
    # order of these loops is important
    for iNumberOfOnes in range(6):
        for iData2Encode in range(256):
            data = calculateNewAx25DataFromOld(iNumberOfOnes, iData2Encode)
            combinations.append(data)
            (oldNumberOfOnes, byte2Encode, newDataGiventLastBitWasZero, newLastBitGiventLastBitWasZero, newDataGiventLastBitWasOne, newLastBitGiventLastBitWasOne, newDataNumberOfBits, newNumberOfOnes) = data
            numberOfZeroes1 = getNumberOfBits(0, newDataGiventLastBitWasZero, newDataNumberOfBits)
            numberOfZeroes2 = getNumberOfBits(0, newDataGiventLastBitWasOne, newDataNumberOfBits)
            maxZeroes = max(maxZeroes, numberOfZeroes1, numberOfZeroes2)
            totalNumberOfZeroes += numberOfZeroes1 + numberOfZeroes2
            numberOfOnes1 = getNumberOfBits(1, newDataGiventLastBitWasZero, newDataNumberOfBits)
            numberOfOnes2 = getNumberOfBits(1, newDataGiventLastBitWasOne, newDataNumberOfBits)
            maxOnes = max(maxOnes, numberOfOnes1, numberOfOnes2)
            totalNumberOfOnes += numberOfOnes1 + numberOfOnes2
    print("maxZeroes = " + str(maxZeroes))
    print("maxOnes = " + str(maxOnes))
    print("totalNumberOfZeroes = " + str(totalNumberOfZeroes))
    print("totalNumberOfOnes = " + str(totalNumberOfZeroes))
    return combinations
