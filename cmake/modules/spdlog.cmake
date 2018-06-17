cmake_minimum_required(VERSION 3.10.3)

project(spdlog NONE)

include(ExternalProject)
ExternalProject_Add(spdlog
  GIT_REPOSITORY    https://github.com/gabime/spdlog.git
  GIT_TAG           v0.16.3
  SOURCE_DIR        "${CMAKE_BINARY_DIR}/spdlog-src"
  BINARY_DIR        "${CMAKE_BINARY_DIR}/spdlog-build"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  INSTALL_COMMAND   ""
  TEST_COMMAND      ""
  )
