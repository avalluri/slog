#ifndef __SLOG_TEST_LOG_LEVEL_H_
#define __SLOG_TEST_LOG_LEVEL_H_

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include <slog/log_level.h>

using namespace slog;

class LogLevelTest: public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE(LogLevelTest);
    CPPUNIT_TEST(testConstruct);
    CPPUNIT_TEST_SUITE_END();

public:
    LogLevelTest() = default;
    ~LogLevelTest() = default;
    void setUp() {};
    void tearDown() {};

protected:

    void testConstruct() {
        LogLevel level(LogLevel::Warning);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("logleve must be warning", level.Get(), LogLevel::Warning);
    }

private:
    /* data */
    LogLevel level{LogLevel::Debug};
};

#endif // __SLOG_TEST_LOG_LEVEL_H_