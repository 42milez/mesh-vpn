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

add_subdirectory(cli)
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
