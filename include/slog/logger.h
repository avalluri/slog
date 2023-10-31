#ifndef __SLOG_LOGGER_H_
#define __SLOG_LOGGER_H_

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <slog/target.h>
#include <slog/decorators.h>
#include <slog/file_target.h>

using namespace std;

namespace slog {
/**
 * Log level used if no log level set for all the loggers
 */
const static LogLevel::level_t DefaultLogLevel = LogLevel::Info;

class Logger {
    using target_ptr_t = shared_ptr<Target>;
public:
    explicit Logger(string name)
        : context_(move(name)) {}

    template <typename It>
    Logger(string name, It begin, It end)
        : context_(name), targets_(begin, end) {}
    
    template <typename It>
    Logger(string name, LogLevel::level_t level, It begin, It end)
        : context_(name), level_(level), targets_(begin, end) {}

    Logger(string name, LogLevel::level_t level, initializer_list<target_ptr_t> targets)
        : Logger{name, level, targets.begin(), targets.end()} {}

    Logger(string name, initializer_list<target_ptr_t> targets)
        : Logger{name, targets.begin(), targets.end()} {}
    
    Logger(string name, target_ptr_t target)
        : Logger{name, {target}} {}

    Logger(string name, LogLevel::level_t level)
        : context_(move(name)), level_(level) {}

    Logger(string name, LogLevel::level_t level, target_ptr_t target)
        : Logger{name, level, {target}} {}

    ~Logger() {};

    const string& Name() const {
        return context_;
    }

    void SetName(string name) {
        context_ = move(name);
    }

    const vector<target_ptr_t>& Targets() const {
        return targets_;
    }

    void AddTarget(target_ptr_t target) {
        std::lock_guard<std::mutex> lock(targets_mtx_);
        targets_.push_back(target);
    }

    void RemoveTarget(target_ptr_t target) {
        std::lock_guard<std::mutex> lock(targets_mtx_);
        for (auto it = targets_.begin(); it != targets_.end(); ++it) {
            if (*it == target) {
                targets_.erase(it);
                break;
            }
        }
    }

    LogLevel::level_t GetLevel() const {
        return level_.Get();
    }

    void SetLevel(LogLevel::level_t lvl) {
        level_ = lvl;
    }

    void Trace(const string& msg) {
        log_entry(LogLevel::Trace, msg);
    }

    void Debug(const string& msg) {
        log_entry(LogLevel::Debug, msg);
    }

    void Info(const string& msg) {
        log_entry(LogLevel::Info, msg);
    }

    void Warning(const string& msg) {
        log_entry(LogLevel::Warning, msg);
    }

    void Error(const string& msg) {
        log_entry(LogLevel::Error, msg);
    }

    void Critical(const string& msg) {
        log_entry(LogLevel::Critical, msg);
    }

protected:
    void log_entry(LogLevel::level_t lvl, const string& msg) {
        // do nothing if log level is not enabled.
        if (LogLevel{lvl} < level_) return;

        // TODO(avalluri): currently using predefined list and order of 
        // log message decorators. This shall be configurable per logger/target.
        std::string decorated_msg = DateTimeDecorator().string() + " " +
            PidDecorator().string() + " " +
            LogLevelDecorator(lvl).string() + " " + msg;

        for (auto target: targets_) {
            target->Log(lvl, decorated_msg);
        }
    }

private:
    string  context_;
    LogLevel level_{DefaultLogLevel};
    vector<shared_ptr<slog::Target> > targets_{make_shared<StdoutTarget<mutex> >(LogLevel::Trace)};
    std::mutex targets_mtx_; // mutex to protect targets_ from concurrent access
}; // class logger

} // namespace slog

#endif // __SLOG_LOGGER_H_