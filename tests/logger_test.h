#ifndef __SLOG_LOGGER_TEST_H_
#define __SLOG_LOGGER_TEST_H_

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include <slog/file_target.h>
#include <slog/logger.h>

using namespace slog;

/**
 * LoggerTest
 *
 * Group of tests to validate slog::Logger interface
*/
class LoggerTest: public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE(LoggerTest);
    CPPUNIT_TEST(testWithDefaltOptions);
    CPPUNIT_TEST(testWithCustomOptions);
    CPPUNIT_TEST_SUITE_END();
public:
    LoggerTest() = default;
    ~LoggerTest() = default;
    void setUp() {
    }
    void tearDown() {
    }
protected:
    void testWithDefaltOptions() {
        Logger l{"test"};
        CPPUNIT_ASSERT_EQUAL(l.GetLevel(), DefaultLogLevel);
        CPPUNIT_ASSERT_EQUAL(l.Name(), std::string("test"));
        CPPUNIT_ASSERT_EQUAL(l.Targets().size(), static_cast<size_t>(1));
        auto t = l.Targets()[0];
        CPPUNIT_ASSERT(typeid(*t) == typeid(slog::StdoutTarget<std::mutex>));
    }

    void testWithCustomOptions() {
        Logger l{"test", LogLevel::Info, std::make_shared<StderrTarget<std::mutex> >()};
        CPPUNIT_ASSERT_EQUAL(l.GetLevel(), LogLevel::Info);
        CPPUNIT_ASSERT_EQUAL(l.Name(), std::string("test"));
        CPPUNIT_ASSERT_EQUAL(l.Targets().size(), static_cast<size_t>(1));
        auto t = l.Targets()[0];
        CPPUNIT_ASSERT(typeid(*t) == typeid(slog::StderrTarget<std::mutex>));
    }
private:
}; // class LoggerTest

#endif // __SLOG_LOGGER_TEST_H_