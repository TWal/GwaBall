#ifndef LOGGER_H
#define LOGGER_H

#include <cstdarg>
#include <chrono>
#include <cstdio>
#include <vector>
#include "Utils.h"

class Logger {
    public:
        enum PRIORITY {
            INFO,
            WARNING,
            ERROR,
            FATAL
        };
        enum OUTPUT {
            STDOUT = bit(0),
            STDERR = bit(1),
            LOGFILE = bit(2)
        };
        Logger(PRIORITY minPrio, char output, const char* file = "");
        ~Logger();
        void info(const char* format, ...);
        void warning(const char* format, ...);
        void error(const char* format, ...);
        void fatal(const char* format, ...);
    private:
        void log(PRIORITY prio, const char* format, va_list args);
        static std::chrono::system_clock::time_point _startClock;
        PRIORITY _minPrio;
        std::vector<FILE*> _outputs;
        FILE* _file;
};

#endif

