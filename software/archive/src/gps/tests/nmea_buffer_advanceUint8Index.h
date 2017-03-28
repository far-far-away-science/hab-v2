#pragma once

#include <test/test.h>

#include "gps/nmea_buffer.h"

TEST_CLASS(nmeaBuffer_advanceUint8Index,
{
    TEST_METHOD(should_return_next_value_if_not_at_the_max_index,
    {
        ASSERT_INT_ARE_EQUAL((uint8_t) 1, advanceUint8Index(0, 5));
        ASSERT_INT_ARE_EQUAL((uint8_t) 4, advanceUint8Index(3, 5));
    })

    TEST_METHOD(should_return_0_value_if_at_or_behind_max_index,
    {
        ASSERT_INT_ARE_EQUAL((uint8_t) 0, advanceUint8Index(4, 5));
        ASSERT_INT_ARE_EQUAL((uint8_t) 0, advanceUint8Index(10, 5));
    })
})
