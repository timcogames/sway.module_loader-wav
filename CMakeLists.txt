#---------------------------------------------------------------------------------
# CMake version
#---------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.25.0 FATAL_ERROR)

#---------------------------------------------------------------------------------
# Project directories
#---------------------------------------------------------------------------------

set(MODULE_LOADER_WAV_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(MODULE_LOADER_WAV_LIST_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(MODULE_LOADER_WAV_CMAKE_DIR "${MODULE_LOADER_WAV_ROOT_DIR}/submodules/sway.module_core/cmake_modules")
set(MODULE_LOADER_WAV_LIB_DIR "${MODULE_LOADER_WAV_ROOT_DIR}/lib")
set(MODULE_LOADER_WAV_BIN_DIR "${CMAKE_SOURCE_DIR}/bin")

#---------------------------------------------------------------------------------
# Include CMake modules
#---------------------------------------------------------------------------------

include("${MODULE_LOADER_WAV_CMAKE_DIR}/options.cmake")
include("${MODULE_LOADER_WAV_CMAKE_DIR}/project_metadata.cmake")
include("${MODULE_LOADER_WAV_CMAKE_DIR}/enable_cxx17.cmake")
include("${MODULE_LOADER_WAV_CMAKE_DIR}/add_submodule.cmake")
include("${MODULE_LOADER_WAV_CMAKE_DIR}/set_compile_definitions.cmake")
include("${MODULE_LOADER_WAV_CMAKE_DIR}/set_output_dir.cmake")
include("${MODULE_LOADER_WAV_CMAKE_DIR}/modules/create_object_library.cmake")
include("${MODULE_LOADER_WAV_CMAKE_DIR}/modules/create_interface.cmake")
include("${MODULE_LOADER_WAV_CMAKE_DIR}/modules/compiler/clang/create_library.cmake")
include("${MODULE_LOADER_WAV_CMAKE_DIR}/modules/compiler/emscripten/setup.cmake")
include("${MODULE_LOADER_WAV_CMAKE_DIR}/modules/compiler/emscripten/create_library.cmake")
include("${MODULE_LOADER_WAV_CMAKE_DIR}/modules/compiler/emscripten/gen_target_name.cmake")
include("${MODULE_LOADER_WAV_CMAKE_DIR}/modules/compiler/emscripten/set_optimization.cmake")
include("${MODULE_LOADER_WAV_CMAKE_DIR}/modules/compiler/emscripten/set_environment.cmake")
include("${MODULE_LOADER_WAV_CMAKE_DIR}/modules/compiler/emscripten/set_compilation.cmake")
include("${MODULE_LOADER_WAV_CMAKE_DIR}/modules/compiler/emscripten/set_modularize.cmake")

#---------------------------------------------------------------------------------
# Опции сборки
#---------------------------------------------------------------------------------

option(MODULE_LOADER_WAV_LIB_TYPE "Build library type" OFF)
option(MODULE_LOADER_WAV_ENVIRONMENT "Environment variable" OFF)
option(MODULE_LOADER_WAV_COMPILATION "Compilation variable" OFF)
option(MODULE_LOADER_WAV_MT "Enable multithreading" OFF)
option(MODULE_LOADER_WAV_ALL_EXPORT "" OFF)
option(MODULE_LOADER_WAV_ENABLE_COVERAGE "Enable coverage builds" OFF)
option(MODULE_LOADER_WAV_ENABLE_TESTS "Enable test builds" OFF)

#---------------------------------------------------------------------------------
# [EMSCRIPTEN]: Setup
#---------------------------------------------------------------------------------

if(GLOB_EMSCRIPTEN_PLATFORM)
  emscripten_setup()
endif()

#---------------------------------------------------------------------------------
# Project
#---------------------------------------------------------------------------------

project_metadata("loader_wav" 0.1.0)
project(${MODULE_LOADER_WAV_PROJ_NAME} VERSION ${MODULE_LOADER_WAV_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=default -fPIE")

#---------------------------------------------------------------------------------
# Submodules
#---------------------------------------------------------------------------------

add_submodule("module_core" submodules/sway.module_core)

#---------------------------------------------------------------------------------
# 
#---------------------------------------------------------------------------------

message("[${MODULE_LOADER_WAV_PROJ_NAME}]: ${MODULE_LOADER_WAV_VERSION}
         compiler | ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}
       build type | ${CMAKE_BUILD_TYPE}
         lib type | ${MODULE_LOADER_WAV_LIB_TYPE}
         cxxflags | ${CMAKE_CXX_FLAGS} ${EXTRA_FLAGS}
 build type flags | ${BUILD_TYPE_CXX_FLAGS}
        build dir | ${CMAKE_BINARY_DIR}
   install prefix | ${CMAKE_INSTALL_PREFIX}

        gtest dir | ${GLOB_GTEST_ROOT_DIR}

         coverage | ${MODULE_LOADER_WAV_ENABLE_COVERAGE}
            tests | ${MODULE_LOADER_WAV_ENABLE_TESTS}
")

#---------------------------------------------------------------------------------
# Определяем каталог вывода по умолчанию
#---------------------------------------------------------------------------------

set_output_dir(${MODULE_LOADER_WAV_BIN_DIR})

#---------------------------------------------------------------------------------
# Добавляем пути для поиска хедеров
#---------------------------------------------------------------------------------

# wav, mp3, flac
include_directories("${GLOB_THIRD_PARTY_DIR}/dr_libs")

# include_directories("/Library/Developer/CommandLineTools/usr/include/c++/v1")
# if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
#   include_directories("/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include")
# endif()
include_directories("${MODULE_LOADER_WAV_LIST_DIR}/lib/cxx/include")
include_directories("${MODULE_LOADER_WAV_LIST_DIR}/submodules/sway.module_loader/api/cxx/include")
include_directories("${MODULE_LOADER_WAV_LIST_DIR}/submodules/sway.module_core/lib/cxx/include")
include_directories("${MODULE_LOADER_WAV_LIST_DIR}/submodules/sway.module_math/lib/cxx/include")

# Получаем пути ко всем хедерам и исходникам библиотеки
file(GLOB_RECURSE MODULE_LOADER_WAV_HEADERS "${MODULE_LOADER_WAV_LIST_DIR}/lib/cxx/include/*.*")
file(GLOB_RECURSE MODULE_LOADER_WAV_SOURCES "${MODULE_LOADER_WAV_LIST_DIR}/lib/cxx/src/*.*")

#---------------------------------------------------------------------------------
# Build object library
#---------------------------------------------------------------------------------

if(NOT TARGET ${MODULE_LOADER_WAV_OBJ_TARGET})
  set(MODULE_LOADER_WAV_LIB_SOURCES "${MODULE_LOADER_WAV_ROOT_DIR}/lib/cxx/src/*.*")
  set(MODULE_LOADER_WAV_ENABLE_FILE_EXPORT TRUE)

  create_object_library(${MODULE_LOADER_WAV_OBJ_TARGET} ${MODULE_LOADER_WAV_LIB_SOURCES} ${MODULE_LOADER_WAV_ENABLE_FILE_EXPORT})
  enable_cxx17(${MODULE_LOADER_WAV_OBJ_TARGET})
endif()

if(MODULE_LOADER_WAV_ENABLE_TESTS OR MODULE_LOADER_WAV_USE_GMOCK)
  target_compile_definitions(${MODULE_LOADER_WAV_OBJ_TARGET} PUBLIC -DRENDER_USE_GMOCK)
endif()

#---------------------------------------------------------------------------------
# Create interfaces
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
# [EMSCRIPTEN] Bitcode
#---------------------------------------------------------------------------------

if(NOT MODULE_LOADER_WAV_LIB_TYPE STREQUAL "object")
  if(GLOB_EMSCRIPTEN_PLATFORM)
    create_emscripten_library(${MODULE_LOADER_WAV_LIB_TARGET} ${MODULE_LOADER_WAV_ENVIRONMENT} ${MODULE_LOADER_WAV_COMPILATION} OUTPUT_TARGET_NAME_LIST)

    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --profiling-funcs")

    foreach(target IN LISTS OUTPUT_TARGET_NAME_LIST)
      target_link_options(${target} PUBLIC 
        "SHELL:-s MODULARIZE=1"
        "SHELL:-s FULL_ES3=1"
        "SHELL:-s SUPPORT_LONGJMP=0"
        "SHELL:-s EXPORT_NAME=\"create_loader_wav_module\""
        "SHELL:-s EXPORTED_RUNTIME_METHODS=@${MODULE_LOADER_WAV_LIB_DIR}/runtime-exports.json")

      # target_link_options(${target} PUBLIC 
      #   "SHELL:-s STRICT=1")
      # OR
      target_link_options(${target} PUBLIC "SHELL:-s BINARYEN_ASYNC_COMPILATION=1")

      if(MODULE_LOADER_WAV_ALL_EXPORT)
        target_link_options(${target} PUBLIC "SHELL:-s LINKABLE=1") # Полный экспорт
      else()
        target_link_options(${target} PUBLIC "SHELL:-s EXPORTED_FUNCTIONS=@${MODULE_LOADER_WAV_LIB_DIR}/exports.json") # Частичный экспорт
      endif()

      if(MODULE_LOADER_WAV_LIB_TYPE STREQUAL "shared")
        target_link_options(${target} PUBLIC 
          "SHELL:-shared"
          "SHELL:-dynamiclib")

        if(MODULE_LOADER_WAV_ALL_EXPORT)
          target_link_options(${target} PUBLIC "SHELL:-s SIDE_MODULE=2")
        else()
          target_link_options(${target} PUBLIC "SHELL:-s SIDE_MODULE=1")
        endif()
      endif()

      if(MODULE_LOADER_WAV_MT)
        target_link_options(${target} PUBLIC 
          "SHELL:-Wl,--shared-memory,--no-check-features"
          "SHELL:-s USE_PTHREADS=1"
          "SHELL:-s PTHREAD_POOL_SIZE=8"
          "SHELL:-s IMPORTED_MEMORY=1")
      endif()

      enable_cxx17(${target})

      printf_link_options(${target})
    endforeach()
  else()
    # set(CMAKE_CXX_COMPILER "/opt/homebrew/Cellar/gcc/13.2.0/bin/g++-13")
    set(CMAKE_CXX_COMPILER "/Library/Developer/CommandLineTools/usr/bin/g++")

    create_library(${MODULE_LOADER_WAV_LIB_TARGET} "${MODULE_LOADER_WAV_ROOT_DIR}/lib/cxx/src/*.*")
    target_link_libraries(${MODULE_LOADER_WAV_LIB_TARGET} module_core_obj)
    enable_cxx17(${MODULE_LOADER_WAV_LIB_TARGET})
  endif()
endif()

# Добавляем построение тестов
if(MODULE_LOADER_WAV_ENABLE_TESTS)
  if(MODULE_LOADER_WAV_LIB_TYPE STREQUAL "shared")
    message(STATUS "[${MODULE_LOADER_WAV_PROJ_NAME}]: Tests are only allowed with a static lib")
  else()
    message(STATUS "[${MODULE_LOADER_WAV_PROJ_NAME}]: Tests have been enabled")
    add_subdirectory(lib/cxx/tests/google)
  endif()
endif()
