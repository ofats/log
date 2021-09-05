#ifndef _OFATS_LOG_OFATS_LOG_SOURCE_LOCATION_H
#define _OFATS_LOG_OFATS_LOG_SOURCE_LOCATION_H

#if 1
#if defined __has_builtin
#if !__has_builtin(__builtin_FILE) || !__has_builtin(__builtin_FUNCTION) || \
    !__has_builtin(__builtin_LINE)
#error "SourceLocation is not supported"
#endif
#else
#error "SourceLocation is not supported"
#endif
#endif

#include <string>

namespace ofats {

class SourceLocation {
 public:
  constexpr SourceLocation(const char* file = __builtin_FILE(),
                           const char* func = __builtin_FUNCTION(),
                           int line = __builtin_LINE())
      : file_(file), func_(func), line_(line) {}

  constexpr const char* file_name() const { return file_; }
  constexpr const char* function_name() const { return func_; }
  constexpr int line() const { return line_; }

  std::string Format() const;

 private:
  const char* file_;
  const char* func_;
  int line_;
};

};  // namespace ofats

#endif
