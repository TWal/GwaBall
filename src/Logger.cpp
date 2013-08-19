#include "Logger.h"

#include <ctime>

std::chrono::system_clock::time_point Logger::_startClock;

Logger::Logger(PRIORITY minPrio, char outputs, const char* file) {
    if(_startClock.time_since_epoch().count() == 0) {
        _startClock = std::chrono::system_clock::now();
    }
    _minPrio = minPrio;
    if(outputs & STDOUT) {
        _outputs.push_back(stdout);
    }
    if(outputs & STDERR) {
        _outputs.push_back(stderr);
    }
    if(outputs & LOGFILE) {
        _file = fopen(file, "w");
        if(_file) {
            _outputs.push_back(_file);
        } else {
            fprintf(stderr, "Cannot open file '%s'.", file);
        }
    } else {
        _file = nullptr;
    }
}

Logger::~Logger() {
    if(_file) {
        fclose(_file);
    }
}

void Logger::info(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(INFO, format, args);
    va_end(args);
}

void Logger::warning(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(WARNING, format, args);
    va_end(args);
}

void Logger::error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(ERROR, format, args);
    va_end(args);
}

void Logger::fatal(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(FATAL, format, args);
    va_end(args);
}

void Logger::log(PRIORITY prio, const char* format, va_list args) {
    if(prio < _minPrio) {
        return;
    }

    va_list argsCopy;
    va_copy(argsCopy, args);
    int len = vsnprintf(nullptr, 0, format, argsCopy) + 1;
    va_end(argsCopy);
    char* message = new char[len];
    vsnprintf(message, len, format, args);

    time_t rawtime;
    time(&rawtime);
    tm* timeinfo = localtime(&rawtime);
    char timeStr[80];
    strftime(timeStr, 80, "%x %X", timeinfo);

    float timeSinceCreation = (std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::system_clock::now() - _startClock)).count();

    for(FILE* output : _outputs) {
        fprintf(output, "[% 13.6f][%s]\t%s\n", timeSinceCreation, timeStr, message);
    }

    delete[] message;
}

