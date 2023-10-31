#ifndef __SLOG_FILE_TARGET_TEST_H_
#define __SLOG_FILE_TARGET_TEST_H_

#include <vector>
#include <thread>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include <slog/file_target.h>
#include <slog/file_exception.h>
#include "test_utils.h"


using namespace slog;

struct null_mutex {
    void lock() {}
    void unlock() {}
};

/**
 * FileTargetTest
 *
 * Group of tests to validate slog::FileTarget interface
*/
class FileTargetTest: public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE(FileTargetTest);
    CPPUNIT_TEST(testFileTargetConstruct);
    CPPUNIT_TEST(testFileTargetSymlink);
    CPPUNIT_TEST(testFileTargetLogs);
    CPPUNIT_TEST(testFileTargetConcurrent);
    CPPUNIT_TEST_SUITE_END();

public:
    FileTargetTest() = default;
    ~FileTargetTest() = default;
    void setUp() {
        // ensure no TEST_DIR exists before
        // running the tests
        cleanupTestdata();
    }
    void tearDown() {
        //::system((std::string("rm -rf ") + TEST_DIR).c_str());
        cleanupTestdata();
    }

protected:
    void testFileTargetConstruct() {
        CPPUNIT_ASSERT_NO_THROW_MESSAGE("FileTarget(log_file)", FileTarget<std::mutex>{test_file_});
        std::ifstream fs(test_file_, std::ifstream::in);
        CPPUNIT_ASSERT_MESSAGE("log file should have created", fs.good());
        CPPUNIT_ASSERT_NO_THROW_MESSAGE("FileTarget(log_file) should not fail to use existing file",
            FileTarget<std::mutex>{test_file_});
    }

    void testFileTargetSymlink() {
        // shoild fail on using a symlik as log file
        auto tmp_file = TEST_FILE("tmp-file");
        ::system((std::string("mkdir ") + TEST_DIR).c_str());
        ::system((std::string("touch ") + tmp_file).c_str());
        // create a symlink
        ::system((std::string("ln -s ") + tmp_file + " " + test_file_).c_str());
        CPPUNIT_ASSERT_THROW_MESSAGE("should fail to use symlink",
            FileTarget<std::mutex>{test_file_}, FileException);
    }

    void testFileTargetLogs() {
        std::string in_msg = "info log message";
        std::string out_msg;
        Target *t = new FileTarget<null_mutex>{test_file_, LogLevel::Info};

        // write a log message to target
        t->Log(LogLevel::Info, in_msg);
        t->Flush();

        // read back the message from log file
        std::ifstream fs(test_file_, std::ifstream::in);
        std::getline(fs, out_msg);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("read log message", in_msg, out_msg);

        // write a log message with a level higher than the target support
        in_msg = "trace log message";
        t->Log(LogLevel::Trace, in_msg);
        t->Flush();

        // retry logging with appropriate level
        in_msg = "error log message";
        t->Log(LogLevel::Error, in_msg);
        t->Flush();

        // try read the next messgae, it should be error messgae
        std::getline(fs, out_msg);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("read log message", in_msg, out_msg);
        delete t;
    }

    void testFileTargetConcurrent() {
        Target *t = new FileTarget<std::mutex>{test_file_, LogLevel::Error};
        auto log_messgae = [=](std::string message) {
            t->Log(LogLevel::Error, message);
        };

        std::vector<std::thread> threads;

        // write cocurrently log messages
        for (int i=0; i < 100; i++) {
            threads.emplace_back(std::thread(log_messgae, std::string("message ") + std::to_string(i)));
        }
        // wait till all threads are completed
        for (auto &t : threads) {
            t.join();
        }
        t->Flush();

        int nLines = 0;
        std::ifstream fs(test_file_, std::ifstream::in);
        for (std::string line; std::getline(fs, line); nLines++) ;
        // check that all 100 lines are found in the log file
        CPPUNIT_ASSERT_EQUAL(100, nLines);

        delete t;
    }

private:
    std::string test_file_{TEST_FILE("test-logs.txt")}; // file name used for testing
}; // class FileTargetTest

#endif // __SLOG_FILE_TARGET_TEST_H_