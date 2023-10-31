/* Copyright (C) 2023 Amarnath Valluri - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: , or visit:
 * https://opensource.org/license/MIT/
 */
#ifndef __SLOG_TARGET_H_
#define __SLOG_TARGET_H_

#include <string>
#include <slog/log_level.h>

namespace slog {

/**
 * Target as base class for implementing logging targets
 * 
 * All targets must be inherited from this, and implement
 * its simple interface consits of:
 *
 *  void log(const string& msg): write the message to target buffer.
 *  void flush():  flush target buffer.
*/
class Target {
public:
    explicit Target(LogLevel lvl): level_(lvl) {}
    Target() = default;
    virtual ~Target() = default;
   
    // GetLogLevel retusn the current log level used by this target
    const LogLevel& GetLogLevel() const  {
        return level_;
    }

    // SetLogLevel update the targets log level
    void SetLogLevel(const LogLevel& level) {
        level_ = level;
    }

    // ShouldLog returns if the messages with the given log level
    // should be logged.
    bool ShouldLog(const LogLevel& level) {
        return level <= level_;
    }

    bool Log(LogLevel::level_t level, const std::string& msg) {
        if (this->ShouldLog(level)) {
            return this->log(msg);
        }
        return true;
    }

    void Flush() {
        this->flush();
    }

protected:
    /**
     * log the give message to target stream, return false if failure to do so.
    */
    virtual bool log(const std::string& msg) = 0;
    /**
     * flush the target stream
    */
    virtual void flush() = 0;

    // Target specific log level.
    // This allows say, to log all warnings to one target, say stdout
    // and all traces to other target(file) etc.,.
    LogLevel level_{LogLevel::None};
}; // class target

} // namespace slog

#endif // __SLOG_TARGET_H_