/* Copyright (C) 2023 Amarnath Valluri - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: , or visit:
 * https://opensource.org/license/MIT/
 */
#ifndef __SLOG_TEST_LOG_LEVEL_H_
#define __SLOG_TEST_LOG_LEVEL_H_

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include <slog/log_level.h>

using namespace slog;

/**
 * LogLevelTest
 *
 * Group of tests to validate slog::LogLevel interface
*/
class LogLevelTest: public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE(LogLevelTest);
    CPPUNIT_TEST(testLogLevelConstruct);
    CPPUNIT_TEST(testLogLevelSetGet);
    CPPUNIT_TEST(testLogLevelComparision);
    CPPUNIT_TEST_SUITE_END();

public:
    LogLevelTest() = default;
    ~LogLevelTest() = default;
    void setUp() {
        level.Set(LogLevel::None);
    };
    void tearDown() {};

protected:

    void testLogLevelConstruct() {
        std::map<std::string, LogLevel::level_t> tests{
            {"none", LogLevel::None},
            {"critical", LogLevel::Critical},
            {"warning", LogLevel::Warning},
            {"error", LogLevel::Error},
            {"info", LogLevel::Info},
            {"debug", LogLevel::Debug},
            {"trace", LogLevel::Trace},
            {"xxxx", LogLevel::Unknown}
        };
        for (const auto& kv : tests) {
            CPPUNIT_ASSERT_EQUAL_MESSAGE("LogLevel(string)", (LogLevel{kv.first}).Get(), kv.second);
        }
    }

    void testLogLevelSetGet() {
        // ensure the log level set to None as in setUp
        CPPUNIT_ASSERT_EQUAL_MESSAGE("log level should set None", level.Get(), LogLevel::None);
        // ensure LogLevel::Set/Get() works as expected
        level.Set(LogLevel::Warning);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("log level should set Warning", level.Get(), LogLevel::Warning);
    }

    void testLogLevelComparision() {
        level.Set(LogLevel::Warning);
        // operator==()
        CPPUNIT_ASSERT_MESSAGE("LogLevel == warning", level == LogLevel::Warning);

        std::vector<LogLevel::level_t> less_than_warning{LogLevel::None, LogLevel::Critical, LogLevel::Error};
        std::vector<LogLevel::level_t> greater_than_warning{LogLevel::Debug, LogLevel::Info, LogLevel::Trace};

        // operator{<,<=}()
        for (auto l : greater_than_warning) {
            CPPUNIT_ASSERT_MESSAGE("LogLevel<()",  level < l);
            CPPUNIT_ASSERT_MESSAGE("LogLevel<=()",  level <= l);
        }
        CPPUNIT_ASSERT_MESSAGE("LogLevel<=()",  level <= LogLevel::Warning);

        // operator{>,>=}()
        for (auto l : less_than_warning) {
            CPPUNIT_ASSERT_MESSAGE("LogLevel>()",  level > l);
            CPPUNIT_ASSERT_MESSAGE("LogLevel>=()",  level >= l);
        }
        CPPUNIT_ASSERT_MESSAGE("LogLevel>=()",  level >= LogLevel::Warning);

        // operator!=()
        for (auto l : less_than_warning) {
            CPPUNIT_ASSERT_MESSAGE("LogLevel!=()",  level != l);
        }
        for (auto l : greater_than_warning) {
            CPPUNIT_ASSERT_MESSAGE("LogLevel!=()",  level != l);
        }
    }

private:
    /* data */
    LogLevel level{LogLevel::None};
};

#endif // __SLOG_TEST_LOG_LEVEL_H_