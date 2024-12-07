#ifndef SWAY_LOADER_WAV_WAVPLUGIN_HPP
#define SWAY_LOADER_WAV_WAVPLUGIN_HPP

#include <sway/core.hpp>
#include <sway/loader.hpp>
#include <sway/math.hpp>

NS_BEGIN_SWAY()
NS_BEGIN(loader)
NS_BEGIN(wav)

class WAVPlugin final : public ImageLoaderPlugin {
public:
#pragma region "Static methods"

#pragma endregion

#pragma region "Ctors/Dtor"

  WAVPlugin() = default;

  DTOR_VIRTUAL_DEFAULT(WAVPlugin);

#pragma endregion

#pragma region "Overridden ImageLoaderPlugin methods"

  MTHD_OVERRIDE(auto loadFromStream(std::ifstream &source) -> ImageDescriptor);

  MTHD_OVERRIDE(auto loadFrom(void *buffer, int size) -> ImageDescriptor);

#pragma endregion

private:
};

NS_END()  // namespace wav
NS_END()  // namespace loader
NS_END()  // namespace sway

#endif  // SWAY_LOADER_WAV_WAVPLUGIN_HPP
