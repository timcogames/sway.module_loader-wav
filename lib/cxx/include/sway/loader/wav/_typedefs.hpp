#ifndef SWAY_LOADER_WAV_TYPEDEFES_HPP
#define SWAY_LOADER_WAV_TYPEDEFES_HPP

#include <sway/loader/wav/_prerequisites.hpp>

NS_BEGIN_SWAY()
NS_BEGIN(loader)
NS_BEGIN(wav)

class WAVPlugin;
namespace WAVPluginTypedefs {
using Ptr_t = WAVPlugin *;
}  // namespace WAVPluginTypedefs

NS_END()  // namespace wav
NS_END()  // namespace loader
NS_END()  // namespace sway

#endif  // SWAY_LOADER_WAV_TYPEDEFES_HPP
