#include "log.h"

#include <gtest/gtest.h>

namespace ofats {
namespace {

TEST(LogTest, SmokeTest) {
  MakeStdoutLog() << "hui"
                  << " pizda  d";

  MakeStderrLog() << "hui"
                  << " pizda  d"
                  << " lol";
}

}  // namespace
}  // namespace ofats
