#### Конфигурируем проект для Google тестов

```console
ctest --preset default-test-macos

# deprecated
cmake -D CMAKE_BUILD_TYPE=Debug \
      -D GLOB_EMSCRIPTEN_ROOT_DIR= \
      -D GLOB_EMSCRIPTEN_PLATFORM=OFF \
      -D GLOB_GTEST_ROOT_DIR=/Users/<USER_NAME>/Documents/Third-party/googletest/googletest \
      -D GLOB_GMOCK_ROOT_DIR=/Users/<USER_NAME>/Documents/Third-party/googletest/googlemock \
      -D GLOB_GTEST_LIB_DIR=/Users/<USER_NAME>/Documents/Third-party/googletest/build/lib \
      -D GLOB_GMOCK_LIB_DIR=/Users/<USER_NAME>/Documents/Third-party/googletest/build/lib \
      -D MODULE_CORE_LIB_TYPE=object \
      -D MODULE_CORE_ENVIRONMENT=node \
      -D MODULE_CORE_COMPILATION=async \
      -D MODULE_LOADER_WAV_ENVIRONMENT=node \
      -D MODULE_LOADER_WAV_COMPILATION=async \
      -D MODULE_LOADER_WAV_LIB_TYPE=object \
      -D MODULE_LOADER_WAV_MT=ON \
      -D MODULE_LOADER_WAV_ENABLE_TESTS=ON \
      -D MODULE_LOADER_WAV_ENABLE_COVERAGE=OFF \
      -D MODULE_LOADER_WAV_ENABLE_EXAMPLES=OFF \
      ../
```

#### Запускаем сборку

```console
cmake --build build --preset default-macos-static-mt-debug

# deprecated
cmake --build ./
```
