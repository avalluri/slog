#ifndef __SLOG_LOGGER_TEST_H_
#define __SLOG_LOGGER_TEST_H_

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include <slog/file_target.h>
#include <slog/logger.h>
#include "test_utils.h"

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
    CPPUNIT_TEST(testLoggingToTarget);
    CPPUNIT_TEST_SUITE_END();

    #define TEST_DIR "testdata"
    #define TEST_FILE(file) (std::string(TEST_DIR) + directory_separator + file)
public:
    LoggerTest() = default;
    ~LoggerTest() = default;
    void setUp() {
        // ensure no TEST_DIR exists before
        // running the tests
        cleanupTestdata();
    }
    void tearDown() {
        cleanupTestdata();
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

    void testLoggingToTarget() {
        Logger l{"test", LogLevel::Info, std::make_shared<FileTarget<std::mutex> >(test_file_, LogLevel::Trace)};
        CPPUNIT_ASSERT_MESSAGE("log level mismath", l.GetLevel() == LogLevel::Info);

        std::string info{"info message"};
        std::string warn{"warning message"};
        std::string error{"error message"};
        std::string msg;
        
        // open file for reading
        std::ifstream fs(test_file_, std::ifstream::in);

        l.Info(info); l.Flush();
        // read back and ensrue its there
        std::getline(fs, msg);
        CPPUNIT_ASSERT_MESSAGE("null string read", msg.length() != 0);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("find info message", hasSuffix(msg, info), true);

        l.Warning(warn); l.Flush();
        // read back and ensrue its there
        std::getline(fs, msg);
        CPPUNIT_ASSERT_MESSAGE("null string read", msg.length() != 0);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("find warn message", hasSuffix(msg, warn), true);

        l.Error(error); l.Flush();
        // read back and ensrue its there
        std::getline(fs, msg);
        CPPUNIT_ASSERT_MESSAGE("null string read", msg.length() != 0);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("find error message", hasSuffix(msg, error), true);

        // should not psss on the debug messges as logger
        // is initalized with Info level
        l.Debug("debug message"); l.Flush();
        // read back and ensrue its there
        std::getline(fs, msg);
        CPPUNIT_ASSERT_MESSAGE("expected a nil string, but read: " + msg, msg.length() == 0);
    }
    // FIXME(avalluri): add more logging tests to cover:
    //  > Multi-target logging
    //  > Concurrent logging
    //  > Concurrent multi-target logging
private:
    std::string test_file_{TEST_FILE("test-logs.txt")}; // file name used for testing
}; // class LoggerTest

#endif // __SLOG_LOGGER_TEST_H_