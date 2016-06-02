import random

def generateByte():
    # sequence of zeroes is the worst accumulator of errors.
    # but it seems this error is reset by the appearance of one bit.
    return random.randint(0, 255)

def generateBytes(bitsCount):
    data = []
    bytesCount = int(bitsCount / 8)
    lastByteBitsCount = bitsCount % 8
    for i in range(bytesCount):
        data.append(generateByte())
    if lastByteBitsCount != 0:
        data.append(generateByte() & ((1 << (lastByteBitsCount)) - 1))
    return data

def saveBytes(data, filePath):
    with open(filePath, 'w+') as f:
        for b in data:
            f.write(str(b) + '\n')

def loadBytes(filePath):
    data = []
    with open(filePath, 'r') as f:
        for l in f:
            data.append(int(l.strip()))
    return data
