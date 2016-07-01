#pragma once

#ifndef TEST
    #define TEST
#endif

#if defined(TEST)
    #include <stdint.h>
    #include <stdbool.h>

    #include "errors.h"

    void TRACE_STRING(const char* pString);
    void TRACE_STRING_NO_EOL(const char* pString);
    void TRACE_FORMAT_STRING(const char* pFormat, ...);

    #define TEST_CLASS(name, body) \
        void name(void) \
        body

    #define TEST_METHOD(name, body) \
        void name(void) \
        body \
        TRACE_STRING_NO_EOL("  Running test method: " #name); \
        TRACE_STRING_NO_EOL(" BEGIN."); \
        name(); \
        TRACE_STRING(" END.");

    #define RUN_TEST_CLASS(name) \
        TRACE_STRING("Running test class: " #name); \
        name();

    #ifdef TEST_DEBUG
        #define HANG() \
            TRACE_STRING("Hanged"); \
            for (;;) \
            { \
            }
    #else
        #define HANG()
    #endif

    #define ASSERT_IS_TRUE(value) AssertIsTrue(value, __LINE__)
    #define ASSERT_IS_FALSE(value) AssertIsFalse(value, __LINE__)

    #define ASSERT_INT_ARE_EQUAL(expected, actual) AssertIntAreEqual(expected, actual, __LINE__)
    #define ASSERT_STR_ARE_EQUAL(pExpected, pActual, size) AssertStrAreEqual(pExpected, pActual, size, __LINE__)

    #define ASSERT_FLOAT_IS_LESS_THAN_OR_EQUAL(value1, value2) AssertFloatIsLessThanOrEqual(value1, value2, __LINE__)

    void AssertIsTrue(bool value, uint32_t lineNumber);
    void AssertIsFalse(bool value, uint32_t lineNumber);

    void AssertIntAreEqual(uint8_t expected, uint8_t actual, uint32_t lineNumber);

    void AssertStrAreEqual(uint8_t* pExpected, uint8_t* pActual, uint16_t size, uint32_t lineNumber);

    void AssertFloatIsLessThanOrEqual(float value1, float value2, uint32_t lineNumber);

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
