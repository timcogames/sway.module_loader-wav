#include <sway/loader/wav/wavplugin.hpp>

#define DR_WAV_IMPLEMENTATION
#include <dr_wav.h>

#define ALIGN(x, a) ((x & ~(a - 1)) + a)

NS_BEGIN(sway)
NS_BEGIN(loader)
NS_BEGIN(wav)

DECLARE_LOADER_PLUGIN(WAVPlugin, "wav", "Victor Timoshin", "WAV Loader",
    "https://github.com/timcogames/sway.module_loader-wav", "MIT", core::Version(1))

namespace detail {
auto onRead(void *userdata, void *output, std::size_t bytes) -> std::size_t {
  return static_cast<WAVPlugin *>(userdata)->read(output, bytes);
}

auto onSeek(void *userdata, i32_t offset, drwav_seek_origin origin) -> u32_t {
  return static_cast<WAVPlugin *>(userdata)->seek(offset, origin);
}
}  // namespace detail

WAVPlugin::~WAVPlugin() { drwav_uninit(&decoder_); }

auto WAVPlugin::loadFrom(void *data, i32_t size) -> AudioDescriptor {
  filedata_.resize(size);
  memcpy(filedata_.data(), data, size);

  auto result = drwav_init_ex(&decoder_, detail::onRead, detail::onSeek, nullptr, this, nullptr, 0, nullptr);
  if (!result) {
    std::cout << "Could not open wav file" << std::endl;
  }

  AudioDescriptor desc;
  desc.channels = decoder_.channels;
  desc.rate = decoder_.sampleRate;

  desc.samples.resize(ALIGN(decoder_.totalPCMFrameCount, 4));
  desc.samples.resize(ALIGN(readSamples(desc.samples.data(), desc.samples.size()), 4));

  return desc;
}

auto WAVPlugin::getBody() const -> std::string {}

auto WAVPlugin::readSamples(f32_t *output, std::size_t samples) -> std::size_t {
  return drwav_read_pcm_frames_f32(&decoder_, samples / decoder_.channels, output) * decoder_.channels;
}

auto WAVPlugin::read(void *output, std::size_t bytes) -> std::size_t {
  auto available = filedata_.size() - inputpos_;
  auto count = std::min(bytes, available);
  memcpy(output, filedata_.data() + inputpos_, count);
  inputpos_ += count;
  return count;
}

auto WAVPlugin::seek(i32_t offset, i32_t origin) -> u32_t {
  if (origin == drwav_seek_origin_start) {
    if (offset < 0 || offset > (i32_t)filedata_.size()) {
      return DRWAV_FALSE;
    }

    inputpos_ = offset;
    return DRWAV_TRUE;
  } else if (origin == drwav_seek_origin_current) {
    if ((offset < 0 && (std::size_t)-offset > inputpos_) || inputpos_ + offset > filedata_.size()) {
      return DRWAV_FALSE;
    }

    inputpos_ = (std::size_t)((i32_t)inputpos_ + offset);
    return DRWAV_TRUE;
  } else {
    return DRWAV_FALSE;
  }
}

NS_END()  // namespace wav
NS_END()  // namespace loader
NS_END()  // namespace sway
