# https://stackoverflow.com/questions/42835837/how-to-clone-specific-submodules-from-boost-using-cmake

set(
  configure_cmd ./bootstrap.sh --prefix=${CMAKE_BINARY_DIR}
                               --includedir=${CMAKE_BINARY_DIR}/include
                               --libdir=${CMAKE_BINARY_DIR}/lib
)

set(install_cmd ./b2 -j2 install)

ExternalProject_Add(boost
  GIT_REPOSITORY    https://github.com/boostorg/boost.git
  GIT_TAG           boost-1.65.1
  BUILD_IN_SOURCE   1
  CONFIGURE_COMMAND ${configure_cmd}
  BUILD_COMMAND     ""
  INSTALL_COMMAND   ${install_cmd}
)

add_dependencies(boost openssl)
