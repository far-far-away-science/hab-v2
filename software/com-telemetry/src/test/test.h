#pragma once

#if defined(TEST)
    #include <stdint.h>
    #include <stdbool.h>

    #include "errors.h"

    void TRACE_STRING(const char* pString);
    void TRACE_STRING_NO_EOL(const char* pString);

    #define TEST_CLASS(name, body) \
        void name(void) \
        body

    #define TEST_METHOD(name, body) \
        void name(void) \
        body \
        TRACE_STRING("  BEGIN: " #name); \
        name(); \
        TRACE_STRING("  END.");

    #define RUN_TEST_CLASS(name) \
        TRACE_STRING("Running test class: " #name); \
        name();

    #ifdef TEST_DEBUG
        #define HANG() \
            TRACE_STRING("    Hanged"); \
            for (;;) \
            { \
            }
    #else
        #define HANG()
    #endif

    #define ASSERT_IS_TRUE(value) assertIsTrue(value, __LINE__)
    #define ASSERT_IS_FALSE(value) assertIsFalse(value, __LINE__)

    #define ASSERT_INT_ARE_EQUAL(expected, actual) assertIntAreEqual(expected, actual, __LINE__)
    #define ASSERT_STR_ARE_EQUAL(pExpected, pActual, size) assertStrAreEqual(pExpected, pActual, size, __LINE__)

    #define ASSERT_FLOAT_IS_LESS_THAN_OR_EQUAL(value1, value2) assertFloatIsLessThanOrEqual(value1, value2, __LINE__)

    void assertIsTrue(bool value, uint32_t lineNumber);
    void assertIsFalse(bool value, uint32_t lineNumber);

    void assertIntAreEqual(uint8_t expected, uint8_t actual, uint32_t lineNumber);

    void assertStrAreEqual(uint8_t* pExpected, uint8_t* pActual, uint16_t size, uint32_t lineNumber);

    void assertFloatIsLessThanOrEqual(float value1, float value2, uint32_t lineNumber);

    void executeTests(void);

    #define EXECUTE_TESTS() \
        executeTests();

#else
    #define HANG()
    #define TEST_CLASS(name, body)
    #define TEST_METHOD(name, body)
    #define RUN_TEST_CLASS(name)
    #define EXECUTE_TESTS()
#endif
