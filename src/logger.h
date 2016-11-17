#pragma once

void log_debug(const char* file, int line, const char* fmt, ...) __attribute_format__(printf, 3, 4);
void log_info(const char* file, int line, const char* fmt, ...) __attribute_format__(printf, 3, 4);
void log_warn(const char* file, int line, const char* fmt, ...) __attribute_format__(printf, 3, 4);
void log_error(const char* file, int line, const char* fmt, ...) __attribute_format__(printf, 3, 4);

#define LOG(level, ...) (log_ ## level (__FILE__, __LINE__, __VA_ARGS__))
