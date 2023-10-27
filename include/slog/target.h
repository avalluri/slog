#ifndef __SLOG_TARGET_H_
#define __SLOG_TARGET_H_

#include <string>
#include <slog/log_level.h>

namespace slog {

class Target {
public:
    explicit Target(LogLevel lvl): level_(lvl) {}
    Target() = default;
    virtual ~Target() = default;
    virtual void log(const std::string& msg) = 0;
    virtual void flush() = 0;

    const LogLevel& GetLogLevel() const  {
        return level_;
    }

    void SetLogLevel(const LogLevel& level) {
        level_ = level;
    }

protected:
    // Target specific log level.
    // This allows to log all warnings to one target, say stdout
    // and all traces to other target(file).
    LogLevel level_{LogLevel::Debug};
}; // class target

} // namespace slog

#endif // __SLOG_TARGET_H_