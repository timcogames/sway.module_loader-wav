#include <sway/core.hpp>
#include <sway/loader/wav/wavplugin.hpp>

#include <gtest/gtest.h>

using namespace sway;

TEST(Plug, info) {
  std::ifstream stream("./bin/test.wav", std::ios::binary);
  ASSERT_TRUE(stream.is_open());

  std::string source((std::istreambuf_iterator<s8_t>(stream)), std::istreambuf_iterator<s8_t>());
  stream.close();
  ASSERT_NE(source.size(), 0);

  auto *plug = new loader::wav::WAVPlugin();
  ASSERT_NE(plug, nullptr);

  auto info = plug->loadFrom(source.data(), source.size());
  std::cout << "channels: " << info.channels << std::endl;
  std::cout << "rate: " << info.rate << std::endl;

  SAFE_DELETE_OBJECT(plug);
}
