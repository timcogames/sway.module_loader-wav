#include <sway/loader/wav/wavplugin.hpp>

#include <iostream>

NS_BEGIN(sway)
NS_BEGIN(loader)
NS_BEGIN(wav)

// clang-format off
DECLARE_LOADER_PLUGIN(WAVPlugin, "wav", 
  "Victor Timoshin", 
  "WAV Loader", 
  "https://github.com/timcogames/sway.module_loader-wav",
  "MIT", 
  core::Version(1))
// clang-format on

NS_END()  // namespace wav
NS_END()  // namespace loader
NS_END()  // namespace sway
