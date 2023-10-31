#ifndef __SLOG_TEST_UTILS_H_
#define __SLOG_TEST_UTILS_H_

#include <string>
#include <cstdlib>
#include <slog/utils.h>
#include <ostream>


#define TEST_DIR "testdata"
#define TEST_FILE(file) (std::string(TEST_DIR) + directory_separator + file)


auto cleanupTestdata = [](){ ::system((std::string("rm -rf ") + TEST_DIR).c_str()); };

auto hasSuffix = [](const std::string& msg, const std::string& suffix) -> bool {
    return msg.length() >= suffix.length() && 
           msg.compare(msg.length()-suffix.length(), suffix.length(), suffix) == 0;
};

#endif // __SLOG_TEST_UTILS_H_