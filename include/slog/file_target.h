#ifndef __SLOG_FILE_TARGET_H_
#define __SLOG_FILE_TARGET_H_

#include <string>
#include <mutex>
#include <stdio.h>
#include <slog/target.h>
#include <slog/utils.h>

using namespace std;

namespace slog {

// Generic file target used for logging to a regular file.
// It does not accept symlinks as log target.
// If the file is not exists then it creates a new one with
// user-only readwrite permissions.
template <typename Mutex>
class FileTarget : public Target {
public:
    explicit FileTarget(const string& file_name, LogLevel::level_t lvl = LogLevel::Debug)
        : Target(lvl), file_name_(file_name) {
    
        if (utils::is_symlink(file_name)) {
            throw "Symlink error: log file cannot be a symbolic link";
        }
        if (!utils::ensure_directory_path(utils::dirname(file_name_))) {
            throw "Failed to create log directory: '" + file_name_ + "'";
        }
        fp_ = fopen(file_name_.c_str(), "ab");
        if (!fp_) {
            throw std::string("Failed to open log file '" + file_name_ + "'");
        }
    }
    // Intended for creating console file targets
    explicit FileTarget(FILE *fp, bool is_console = true)
        : fp_(fp), console_(is_console) {}
    
    // Do not support copying/assigning objects
    FileTarget(const FileTarget &) = delete;
    FileTarget(FileTarget &&) = delete;
    FileTarget &operator=(const FileTarget &) = delete;
    FileTarget &operator=(FileTarget &&) = delete;


    void log(const string& msg) override{
        lock_guard<Mutex> lock(mutex_);
        fwrite(msg.c_str(), 1, msg.size(), fp_);
        // Append a new line charechter if needed
        // NOTE(avalluri): make it configurable?
        if (msg.empty() || msg[msg.size()-1] != '\n') {
            fwrite("\n", 1, 1, fp_);
        }
    }

    void flush() override {
        lock_guard<Mutex> lock(mutex_);
       ::fflush(fp_);
    }

    virtual ~FileTarget() {
        if (fp_ != nullptr && ! console_) {
            fclose(fp_);
            fp_ = nullptr;
        }
    }

protected:
    Mutex mutex_;
    string file_name_;
    FILE*  fp_{nullptr};
    bool   console_{false}; // track if the file is a console or not
}; // class FileTarget


// Logging target for standared output
template<class Mutex>
class StdoutTarget: public slog::FileTarget<Mutex> {
public:
    explicit StdoutTarget(): FileTarget<Mutex>(stdout) {}
    explicit StdoutTarget(LogLevel::level_t level): FileTarget<Mutex>(stdout) {
        this->SetLogLevel(level);
    }
}; // class StdoutTarget


// Logging target for standared output
template<class Mutex>
class StderrTarget: public slog::FileTarget<Mutex> {
public:
    explicit StderrTarget(): FileTarget<Mutex>(stderr) {}
    explicit StderrTarget(LogLevel::level_t level): FileTarget<Mutex>(stderr) {
        this->SetLogLevel(level);
    }
}; // class StderrTarget



} // namespace slog

#endif // __SLOG_FILE_TARGET_H_