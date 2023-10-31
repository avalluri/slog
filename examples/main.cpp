#include <iostream>
#include <sstream>
#include <string>
#include <slog/logger.h>
#include <slog/file_target.h>

/**
  * Sample application to illustrate the usage of `slog::Logger`API features.
  *
  * > Logging to multiple targets
  * > Multiple loggers sharing the same target
  * > Filter messages to different targets based on the log level.
  * > Logging user-defined types.
  */
int main(int argc, char *argv[])
{
    MAYBE_UNUSED(argc);
    MAYBE_UNUSED(argv);

    using out_target_t = slog::StdoutTarget<std::mutex>;
    using err_target_t = slog::StderrTarget<std::mutex>;
    using file_target_t = slog::FileTarget<std::mutex>;

    try {

    // looger with default stdout target
    slog::Logger dlog{std::string("default")};
    dlog.Info("log message should be visible on stdout!");

    // Select one or more log targets to write logs to
    auto stdout = std::make_shared<out_target_t>(slog::LogLevel::Trace);
    auto stderr = std::make_shared<err_target_t>(slog::LogLevel::Error);
    auto file = std::make_shared<file_target_t>("logs/sanple-app.log", slog::LogLevel::Trace);

    // Initialize the logger with the required targets
    slog::Logger log{std::string("custom"), {stdout, stderr, file}};

    /**
     * log messages using the API provided by the
     * logger
    */

    log.Error("Unknown error occurred!!! Should be visible on stderr");
    log.Info("some info message to file");
    
    // Could log custom/user-defined types using string streams
    std::ostringstream stream;
    stream << "Sample debug message..." << endl ;
    log.Debug(stream.str());

    /**
     * Two loggers sharing the same (file) target.
    */
    slog::Logger log2{"new_logger", file};
    log2.Debug("This message is from logger2");
    
    } catch(slog::FileException &exp) {
        // Exception might be expected by the FileTarget
        // if it fails to open (or logs to) the log file.
        std::cout << "Exception: "<< exp.what();
    }
}
