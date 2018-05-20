cmake_minimum_required(VERSION 3.9.6)

#set(CMAKE_VERBOSE_MAKEFILE 1)

if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/toolchain.cmake)
endif()

set(CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake" CACHE PATH "The path to the cmake directory")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_DIR})

# project name and version should be set after cmake_policy CMP0048
project(mesh-vpn VERSION "0.0.1")

include(Options)
include(Dependencies)
include(Utils)

configure_project()

add_subdirectory(mvpn)
add_subdirectory(mvcore)
add_subdirectory(mvcrypt)
add_subdirectory(mvutil)

include(ExternalProject)

#  OpenSSL
# --------------------------------------------------
include(cmake/modules/openssl.cmake)

#  Boost
# --------------------------------------------------
include(cmake/modules/boost.cmake)

#  libtorrent
# --------------------------------------------------
include(cmake/modules/libtorrent.cmake)

#  Google Test
# --------------------------------------------------
configure_file(cmake/modules/gtest.cmake googletest-download/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)

execute_process(COMMAND ${CMAKE_COMMAND} --build .
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This adds
# the following targets: gtest, gtest_main, gmock
# and gmock_main
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
  ${CMAKE_BINARY_DIR}/googletest-build)

# The gtest/gmock targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
      "${gmock_SOURCE_DIR}/include")
endif()

enable_testing()
add_subdirectory(test)

#  spdlog
# --------------------------------------------------
configure_file(cmake/modules/spdlog.cmake spdlog-download/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} .
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/spdlog-download)

execute_process(COMMAND ${CMAKE_COMMAND} --build .
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/spdlog-download)

add_subdirectory(${CMAKE_BINARY_DIR}/spdlog-src
  ${CMAKE_BINARY_DIR}/spdlog-build)

include_directories(${CMAKE_BINARY_DIR}/spdlog-src/include)
