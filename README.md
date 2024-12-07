# sway.module_loader-wav

#### Thirdparty

https://github.com/mackron/dr_libs
https://github.com/lieff/minimp3

#### Конфигурируем проект для Google тестов

```console
cmake --preset emscripten-dynamic-mt-release -B build
cmake --preset macos-dynamic-mt-release -B build

# deprecated
cmake -D CMAKE_BUILD_TYPE=Release \
      -D GLOB_EMSCRIPTEN_ROOT_DIR=/Users/<USER_NAME>/Documents/Third-party/emsdk/upstream/emscripten \
      -D GLOB_EMSCRIPTEN_PLATFORM=ON \
      -D GLOB_GTEST_ROOT_DIR= \
      -D GLOB_GMOCK_ROOT_DIR= \
      -D GLOB_GTEST_LIB_DIR= \
      -D GLOB_GMOCK_LIB_DIR= \
      -D MODULE_CORE_ENVIRONMENT=node \
      -D MODULE_CORE_COMPILATION=async \
      -D MODULE_LOADER_WAV_ENVIRONMENT=node \
      -D MODULE_LOADER_WAV_COMPILATION=async \
      -D MODULE_LOADER_WAV_SHARED_LIB=ON \
      -D MODULE_LOADER_WAV_MT=ON \
      -D MODULE_LOADER_WAV_ENABLE_TESTS=OFF \
      -D MODULE_LOADER_WAV_ENABLE_COVERAGE=OFF \
      -D MODULE_LOADER_WAV_ENABLE_EXAMPLES=OFF \
      ../
```

#### Запускаем сборку

```console
cmake --build build --preset default-emscripten-dynamic-mt-release
cmake --build build --preset default-macos-dynamic-mt-release

# deprecated
cmake --build ./
```

#### Таблица символов

```console
# macos
nm -g ./../bin/module_loader_wav.dylib.*
```
