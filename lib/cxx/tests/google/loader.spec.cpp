#include <sway/core.hpp>
#include <sway/loader/wav/wavplugin.hpp>

#include <gtest/gtest.h>

using namespace sway;

TEST(Plug, info) {
  auto *plug = new loader::wav::WAVPlugin();
  plug->loadFrom(nullptr, 0);
}
