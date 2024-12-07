#ifndef SWAY_LOADER_WAV_WAVPLUGIN_HPP
#define SWAY_LOADER_WAV_WAVPLUGIN_HPP

#include <sway/loader/_typedefs.hpp>
#include <sway/loader/wav/_prerequisites.hpp>

NS_BEGIN_SWAY()
NS_BEGIN(loader)
NS_BEGIN(wav)

class WAVPlugin final : public AudioLoaderPlugin {
public:
#pragma region "Ctors/Dtor"

  WAVPlugin() = default;

  DTOR_VIRTUAL(WAVPlugin);

#pragma endregion

#pragma region "Overridden ImageLoaderPlugin methods"

  MTHD_OVERRIDE(auto loadFrom(void *data, i32_t size) -> AudioDescriptor);

  MTHD_OVERRIDE(auto getBody() const -> std::string);

#pragma endregion

  auto readSamples(f32_t *output, std::size_t samples) -> std::size_t;

  auto read(void *output, std::size_t bytes) -> std::size_t;

  auto seek(i32_t offset, i32_t origin) -> u32_t;

private:
  std::vector<u8_t> filedata_;
  std::size_t inputpos_ = 0;
  drwav decoder_ = {};
};

NS_END()  // namespace wav
NS_END()  // namespace loader
NS_END()  // namespace sway

#endif  // SWAY_LOADER_WAV_WAVPLUGIN_HPP
