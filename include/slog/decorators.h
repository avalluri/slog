/* Copyright (C) 2023 Amarnath Valluri - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: , or visit:
 * https://opensource.org/license/MIT/
 */
#ifndef __SLOG_DECORATORS_H_
#define __SLOG_DECORATORS_H_

#include <unistd.h> // ::get_pid()
#include <string>  // std::string
#include <thread>  // std::this_thread::get_pid()
#include <sstream> // std::ostringstream
#include <ctime>   // std::time()
#include <iomanip> // std::put_time()


namespace slog {

// Decorator is base class for an abstract log message decorator.
// Decotratos are supposed to prefix the log messages with specific string.
class Decorator {
public:
    virtual ~Decorator() = default;
    virtual std::string string() = 0;
};

class PidDecorator: public Decorator {
public:
    PidDecorator() = default;

    std::string string() {
        return "[" + std::to_string(static_cast<int>(::getpid())) + "]";
    }
};

class ThreadidDecorator: public Decorator {
public:
    ThreadidDecorator() = default;

    std::string string() {
        std::ostringstream oss;
        oss << "[" <<std::this_thread::get_id() << "]";
        return oss.str();
    }
};

class DateTimeDecorator: public Decorator {
public:
    DateTimeDecorator(): time_(std::time(nullptr)) {};
    explicit DateTimeDecorator(std::time_t t): time_(t) {};

    std::string string() {
        std::ostringstream oss;
        std::tm *tm = std::localtime(&time_);
        oss << "[" <<std::put_time(tm, "%c") << "]";
        return oss.str();
    }
private:
    std::time_t time_;
};

class LogLevelDecorator: public Decorator {
public:
    LogLevelDecorator(LogLevel lvl): level_(lvl) {};
    LogLevelDecorator(LogLevel::level_t lvl): level_(LogLevel(lvl)) {};
    
    std::string string() {
        std::ostringstream oss;
        oss << "[" << level_.ToChar() << "]";
        return oss.str();
    }
private:
    LogLevel level_;
};

} // namespac slog

#endif // __SLOG_DECORATORS_H_