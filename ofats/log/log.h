#ifndef _OFATS_LOG_OFATS_LOG_LOG_H
#define _OFATS_LOG_OFATS_LOG_LOG_H

#include <iostream>
#include <type_traits>

#include "absl/strings/str_cat.h"
#include "absl/time/clock.h"
#include "absl/time/time.h"
#include "source_location.h"

namespace ofats {

template <typename T>
class LogNode {
 public:
  LogNode(const T& left, const absl::AlphaNum& aln) : left_(left), aln_(aln) {}
  ~LogNode() {
    if (last_) {
      left_.Output(aln_);
    }
  }

  template <typename... S>
  void Output(const S&... s) const {
    left_.Output(aln_, s...);
  }

  LogNode<LogNode> operator<<(const absl::AlphaNum& aln) {
    last_ = false;
    return LogNode<LogNode>(*this, aln);
  }

 private:
  const T& left_;
  const absl::AlphaNum& aln_;
  bool last_ = true;
};

template <typename F>
class LogAdapter {
 public:
  explicit LogAdapter(F&& f) : f_(std::forward<F>(f)) {}

  template <typename... S>
  void Output(const S&... s) const {
    f_(s...);
  }

  LogNode<LogAdapter> operator<<(const absl::AlphaNum& aln) {
    return LogNode<LogAdapter>(*this, aln);
  }

 private:
  F f_;
};

template <typename F>
auto MakeLogAdapter(F&& f) -> LogAdapter<F> {
  return LogAdapter<F>(std::forward<F>(f));
}

struct LogToStd {
  template <typename... S>
  void operator()(const S&... s) const {
    sink << absl::StrCat(
                absl::FormatTime("%F %T %Z", absl::Now(), absl::UTCTimeZone()),
                " ", loc.file_name(), ":", loc.function_name(),
                "():", loc.line(), " ", s...)
         << std::endl;
  }

  std::ostream& sink;
  SourceLocation loc;
};

auto MakeStdoutLog(SourceLocation loc = {})
    -> decltype(MakeLogAdapter(std::declval<LogToStd>())) {
  return MakeLogAdapter(LogToStd{
      .sink = std::cout,
      .loc = loc,
  });
}

auto MakeStderrLog(SourceLocation loc = {})
    -> decltype(MakeLogAdapter(std::declval<LogToStd>())) {
  return MakeLogAdapter(LogToStd{
      .sink = std::cerr,
      .loc = loc,
  });
}

}  // namespace ofats

#endif
