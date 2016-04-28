#include "test.h"

#include "gps/tests/nmeaBuffer_advanceUint8Index.h"

void ASSERT_IS_TRUE(bool value)
{
    if (!value)
    {
        ERROR_TEST_FAILED();
        HANG();
    }
}

void ASSERT_ARE_EQUAL(uint8_t expected, uint8_t actual)
{
    if (expected != actual)
    {
        ERROR_TEST_FAILED();
        HANG();
    }
}

void executeTests()
{
    RUN_TEST_CLASS(nmeaBuffer_advanceUint8Index);
}
