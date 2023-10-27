#ifndef __SLOG_LOG_LEVEL_H_
#define __SLOG_LOG_LEVEL_H_

#include <string>
#include <array>
#include <ostream>
#include <iostream>
#include <map>

namespace slog {

class LogLevel {
public:
    enum level_t {
        None,
        Critical,
        Error,
        Warning,
        Info,
        Debug,
        Trace,
        Max,
        Unknwon = Max
    };
    
    LogLevel(int l): lvl_(static_cast<level_t>(l)) {}
    explicit LogLevel(const std::string& level_name)
        : lvl_(LogLevel::from_str(level_name.c_str())) {}
    
    bool operator>(LogLevel& other) const {
        return lvl_ > other.lvl_;
    }

    bool operator>(level_t other) const {
        return lvl_ > other;
    }

    bool operator>=(LogLevel& other) const {
        return lvl_ >= other.lvl_;
    }

    bool operator>=(level_t other) const {
        return lvl_ >= other;
    }

    bool operator<(LogLevel& other) const {
        return lvl_ < other.lvl_;
    }

    bool operator<(level_t other) const {
        return lvl_ < other;
    }

    bool operator<=(LogLevel& other) const {
        return lvl_ <= other.lvl_;
    }

    bool operator<=(level_t other) const {
        return lvl_ <= other;
    }

    bool operator==(LogLevel& other) const {
        return lvl_ == other.lvl_;
    }

    bool operator==(level_t other) const {
        return lvl_ == other;
    }

    const std::string ToString() const {
        return LogLevel::to_str(lvl_);
    }

    char ToChar() const {
        return LogLevel::to_char(lvl_);
    }

private:
    friend std::ostream& operator<<(std::ostream& stream, const LogLevel& l);
    
    static const char* to_str(level_t l);
    static char to_char(level_t l);
    static level_t from_str(const char *level_name);

private:
    level_t lvl_;
}; // class LogLevel

std::ostream& operator<<(std::ostream& stream, const LogLevel& l);

} // namespace slog

#endif // __SLOG_LOG_LEVEL_H_