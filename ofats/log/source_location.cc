#include "source_location.h"

#include <string>

#include "absl/strings/str_format.h"

namespace ofats {

std::string SourceLocation::Format() const {
  return absl::StrFormat("%s:%s():%d ", file_, func_, line_);
}

}  // namespace ofats
