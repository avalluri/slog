#include <ostream>
#include <slog/log_level.h>

namespace slog {

const char* LogLevel::to_str(LogLevel::level_t l) {
    static const std::array<const char *, Max> levels{ 
        {"none", "critical", "error", "warning", "info", "debug", "trace"}
    };

    return (l < None || l >= Max) ? "unknown" : levels[l];
}

char LogLevel::to_char(LogLevel::level_t l) {
    static const std::array<char, Max> levels{ 
        {'_', 'C', 'E', 'W', 'I', 'D', 'T'}
    };

    return (l < None || l >= Max) ? '_': levels[l];
}

LogLevel::level_t LogLevel::from_str(const char *level_name) {
    static const std::map<std::string, level_t> name_to_level{ 
        {"none", None}, {"critical", Critical}, {"error", Error},
        {"warning", Warning}, {"info", Info}, {"debug", Debug},
        {"trace", Trace}
    };

    if (name_to_level.count(level_name) == 1) {
        return name_to_level.find(level_name)->second;
    }
    
    return Unknown;
}    

std::ostream& operator<<(std::ostream& stream, const LogLevel& l) {
    stream << LogLevel::to_str(l.lvl_);
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const LogLevel::level_t l) {
    stream << LogLevel::to_str(l);
    return stream;
}

} // namespace slog
