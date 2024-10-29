#ifndef LOGGER_H
#define LOGGER_H

#include <ctime>
#include <string>
#include <iostream>
#include <glad/glad.h>
#include <ctime>
#include <cstdio>

void log_message(const char* s1, const char* s2, const char* file, int line);
void log_error(const char* s1, const char* s2, const char* file, int line);
std::string get_gl_error();

#define LOG(level, tag, format, ...) do { \
    time_t now = time(0); \
    struct tm* timeinfo = localtime(&now); \
    char timeStr[25]; \
    strftime(timeStr, sizeof(timeStr), "%a %b %d %H:%M:%S %Y", timeinfo); \
    const char* filename = strrchr(__FILE__, '/'); \
    filename = filename ? filename + 1 : __FILE__; \
    printf("[%s %s:%d] %s %s: " format "\n", \
        timeStr, filename, __LINE__, level, tag, ##__VA_ARGS__); \
} while(0)

#define INFO(s1, s2) log_message(s1, s2, __FILE__, __LINE__);
#define ERROR(s1, s2) log_error(s1, s2, __FILE__, __LINE__);

#endif