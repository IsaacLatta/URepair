#ifndef LOGGER_H
#define LOGGER_H

#include <ctime>
#include <string>
#include <iostream>
#include <glad/glad.h>
//#include <GL/gl.h>

void log_message(const char* s1, const char* s2, const char* file, int line);
void log_error(const char* s1, const char* s2, const char* file, int line);
std::string get_gl_error();

#define INFO(s1, s2) log_message(s1, s2, __FILE__, __LINE__);
#define ERROR(s1, s2) log_error(s1, s2, __FILE__, __LINE__);

#endif