#pragma once

#include <cstdarg>
#include <cstdio>

#ifndef RELEASE
#define LOG(...) log("\x1b[92mINFO\x1b[0m", __FILE__, __LINE__, __VA_ARGS__)
#define ERROR(...) log("\x1b[91mERROR\x1b[0m", __FILE__, __LINE__, __VA_ARGS__)
#else
#define LOG(...) \
  {}
#define ERROR(...) \
  {}
#endif

void log(const char* level, const char* fileName, int lineNo, const char* s,
         ...) {
  va_list args;
  va_start(args, s);

  printf("[%s][\x1b[93m%s\x1b[39m:\x1b[96m%d\x1b[0m] \x1b[94m", level, fileName,
         lineNo);
  vprintf(s, args);
  printf("\x1b[0m\n");

  va_end(args);
}
