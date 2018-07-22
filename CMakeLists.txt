cmake_minimum_required(VERSION 3.10.3)

if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/toolchain.cmake)
endif()

set(CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake" CACHE PATH "The path to the cmake directory")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_DIR})

set(HUNTER_CONFIGURATION_TYPES Release)
set(HUNTER_JOBS_NUMBER 4)

include("cmake/HunterGate.cmake")
HunterGate(
  URL "https://github.com/42milez/hunter/archive/v0.23.0a.tar.gz"
  SHA1 "01af060382c1fba2eca1e72b3a3741c75a2597b0"
  LOCAL
)

# project name and version should be set after cmake_policy CMP0048
project(mesh-vpn VERSION "0.0.1")

#  OpenSSL
# --------------------------------------------------
hunter_add_package(OpenSSL)
find_package(OpenSSL REQUIRED)

#  Boost
# --------------------------------------------------
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
hunter_add_package(Boost COMPONENTS chrono filesystem random system thread)
find_package(Boost CONFIG REQUIRED chrono filesystem random system thread)

#  libtorrent
# --------------------------------------------------
hunter_add_package(libtorrent)

#  spdlog
# --------------------------------------------------
hunter_add_package(spdlog)
find_package(spdlog CONFIG REQUIRED)

#  Google Test
# --------------------------------------------------
add_subdirectory(test)

#  mesh-vpn
# --------------------------------------------------
include(Options)
include(Dependencies)
include(Utils)

configure_project()

add_subdirectory(mvpn)
add_subdirectory(mvcore)
add_subdirectory(mvcrypt)
add_subdirectory(mvnetwork)
add_subdirectory(mvutil)
