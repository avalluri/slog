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

class Logger {
public:
    explicit Logger(string name)
        : context_(move(name)) {}

    template <typename It>
    Logger(string name, It begin, It end)
        : context_(name), targets_(begin, end) {}

    Logger(string name, initializer_list<shared_ptr<Target> > targets)
        : Logger{name, targets.begin(), targets.end()} {}

    Logger(string name, shared_ptr<Target> target)
        : Logger{name, {target}} {}

    ~Logger() {};

    const string& context() const {
        return context_;
    }

    template<typename... Args>
    void deubgf(string& fmt, Args &&... args) {
        //log_entry(LogLevel::Deubg, fmt, forward<Args>(args)...);
    }

    void trace(const string& msg) {
        log_entry(LogLevel::Trace, msg);
    }

    void deubg(const string& msg) {
        log_entry(LogLevel::Debug, msg);
    }

    void info(const string& msg) {
        log_entry(LogLevel::Info, msg);
    }

    void warning(const string& msg) {
        log_entry(LogLevel::Warning, msg);
    }

    void error(const string& msg) {
        log_entry(LogLevel::Error, msg);
    }

    void critical(const string& msg) {
        log_entry(LogLevel::Critical, msg);
    }

protected:
    void log_entry(LogLevel::level_t level, const string& msg) {
        // TODO(avalluri): currently using predefined list and order of 
        // log message decorators. This shall be configurable per logger/target.
        
        std::string decorated_msg = DateTimeDecorator().string() + " " +
            PidDecorator().string() + " " +
            LogLevelDecorator(level).string() + " " + msg;

        for (auto target: targets_) {
            if (target->GetLogLevel() >= level) {
                target->log(decorated_msg);
            }
        }
    }

private:
    string  context_;
    vector<shared_ptr<slog::Target> > targets_{make_shared<StdoutTarget<mutex> >(LogLevel::Trace)};
}; // class logger

} // namespace slog

#endif // __SLOG_LOGGER_H_