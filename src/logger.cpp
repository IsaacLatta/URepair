#include "logger.h"

static std::string get_filename(const char* file_path) {
    std::string path(file_path);
    std::size_t pos = path.find_last_of("/\\");  
    if (pos == std::string::npos) {
        return path;  
    }
    return path.substr(pos + 1);  
}

static std::string get_time() {
    std::time_t time = std::time(nullptr);
    std::string time_str = std::ctime(&time);
    time_str.pop_back();
    return time_str;
}

std::string get_gl_error() {
    GLenum err;
    std::string error = "";
    while((err = glGetError()) != GL_NO_ERROR) {
        switch(err) {
            case GL_INVALID_ENUM:      error += " INVALID_ENUM"; break;
            case GL_INVALID_VALUE:     error += " INVALID_VALUE"; break;
            case GL_INVALID_OPERATION: error += " INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:    error += " STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:   error += " STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:     error += " OUT_OF_MEMORY"; break;
            default:                   error += " UNKNOWN_ERROR"; break;
        }
    }
    return error;
}

void log_message(const char* s1, const char* s2, const char* file, int line) {
    std::string time = get_time();
    std::cout << "[" << time << " " << get_filename(file) << ":" << line << "] INFO " << s1 << ": " << s2 << "\n";
}

void log_error(const char* s1, const char* s2, const char* file, int line) {
    std::string time =  get_time();
    std::cerr << "[" << time << " " << get_filename(file) << ":" << line << "] ERROR " << s1 << ": " << s2 << "\n";
}
