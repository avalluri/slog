/* Copyright (C) 2023 Amarnath Valluri - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: , or visit:
 * https://opensource.org/license/MIT/
 */
#ifndef __SLOG_LOG_LEVEL_H_
#define __SLOG_LOG_LEVEL_H_

#include <string>
#include <array>
#include <ostream>
#include <iostream>
#include <map>

namespace slog {

/**
 * LogLevel
 *
 * A convenient interface to export a set of log levels used
 * for logging.
 * 
*/
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
        Unknown = None
    };
    
    LogLevel(const level_t l): lvl_((l < None || l>= Max) ? Unknown : l) {}

    LogLevel(const int l): LogLevel(static_cast<level_t>(l)) {}

    explicit LogLevel(const std::string& level_name)
        : lvl_(LogLevel::from_str(level_name.c_str())) {}
    
    // Get returns the current log level
    constexpr level_t Get() const {
        return lvl_;
    }

    // Set updates the current log level to new_level
    void Set(level_t new_level) {
        lvl_ = new_level;
    }

    // ToString returns the string representation
    // of the current log level in lowercase.
    const std::string ToString() const {
        return LogLevel::to_str(lvl_);
    }

    // ToChar returns the single ASCII character
    // in uppercase that represents the current log level:
    //    None | Unknown => '_'
    //    Critical => 'C'
    //    Error => 'E'
    //    Warning => 'W'
    //    Info => 'I'
    //    Debug => 'D'
    //    Trace => 'T'
    char ToChar() const {
        return LogLevel::to_char(lvl_);
    }

    /**
     * Operator functions
    */

    void operator=(const LogLevel::level_t level) {
        lvl_ = level;
    }

    constexpr bool operator>(const LogLevel& other) const {
        return lvl_ > other.lvl_;
    }

    constexpr bool operator>=(const LogLevel& other) const {
        return lvl_ >= other.lvl_;
    }

    constexpr bool operator<(const LogLevel& other) const {
        return lvl_ < other.lvl_;
    }

    constexpr bool operator<=(const LogLevel& other) const {
        return lvl_ <= other.lvl_;
    }

    constexpr bool operator==(const LogLevel& other) const {
        return lvl_ == other.lvl_;
    }

    constexpr bool operator!=(const LogLevel& other) const {
        return lvl_ != other.lvl_;
    }

private:
    friend std::ostream& operator<<(std::ostream& stream, const LogLevel& l);
    friend std::ostream& operator<<(std::ostream& stream, const level_t l);
    
    static const char* to_str(level_t l);
    static char to_char(level_t l);
    static level_t from_str(const char *level_name);

private:
    level_t lvl_;
}; // class LogLevel

std::ostream& operator<<(std::ostream& stream, const LogLevel& l);
std::ostream& operator<<(std::ostream& stream, const LogLevel::level_t l);

} // namespace slog

#endif // __SLOG_LOG_LEVEL_H_
