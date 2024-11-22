/**
 * @file logger.h
 * @brief Provides utilities for logging messages, errors, and OpenGL errors.
 *
 * The logger module includes functions and macros for logging messages with timestamps,
 * file names, and line numbers. It also provides utilities for retrieving OpenGL error
 * information.
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <ctime>
#include <string>
#include <iostream>
#include <glad/glad.h>
#include <cstdio>

/**
 * @brief Logs an informational message to the console.
 *
 * @param s1 The tag or category of the log message.
 * @param s2 The content of the message to log.
 * @param file The source file where the log call is made.
 * @param line The line number in the source file where the log call is made.
 */
void log_message(const char* s1, const char* s2, const char* file, int line);

/**
 * @brief Logs an error message to the console.
 *
 * @param s1 The tag or category of the log message.
 * @param s2 The content of the error message to log.
 * @param file The source file where the log call is made.
 * @param line The line number in the source file where the log call is made.
 */
void log_error(const char* s1, const char* s2, const char* file, int line);

/**
 * @brief Retrieves OpenGL error messages.
 *
 * Iterates through all OpenGL errors and compiles them into a single string.
 *
 * @return A string describing the OpenGL errors encountered.
 */
std::string get_gl_error();

/**
 * @brief Macro for logging formatted messages.
 *
 * Logs a formatted message with a timestamp, file name, line number, and log level.
 * 
 * @param level The log level (e.g., "INFO", "ERROR").
 * @param tag A tag or category for the log message.
 * @param format The format string for the log message.
 * @param ... Additional arguments for formatting.
 */
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

/**
 * @brief Macro for logging informational messages.
 *
 * Uses `log_message` to log a message at the INFO level with the source file and line number.
 *
 * @param s1 The tag or category of the log message.
 * @param s2 The content of the message to log.
 */
#define INFO(s1, s2) log_message(s1, s2, __FILE__, __LINE__);

/**
 * @brief Macro for logging error messages.
 *
 * Uses `log_error` to log a message at the ERROR level with the source file and line number.
 *
 * @param s1 The tag or category of the error message.
 * @param s2 The content of the error message to log.
 */
#define ERROR(s1, s2) log_error(s1, s2, __FILE__, __LINE__);

#endif
