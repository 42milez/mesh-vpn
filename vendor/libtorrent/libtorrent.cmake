set(
  configure_cmd ./bootstrap.sh --prefix=${CMAKE_BINARY_DIR}
                               --includedir=${CMAKE_BINARY_DIR}/include
                               --libdir=${CMAKE_BINARY_DIR}/lib
                               --with-boost=${CMAKE_BINARY_DIR}
                               --with-openssl=${CMAKE_BINARY_DIR}
)

set(build_cmd make -j2)

ExternalProject_Add(libtorrent
  GIT_REPOSITORY    https://github.com/arvidn/libtorrent.git
  GIT_TAG           libtorrent-1_1_5
  BUILD_IN_SOURCE   1
  CONFIGURE_COMMAND ${configure_cmd}
  BUILD_COMMAND     ${build_cmd}
)

add_dependencies(libtorrent openssl boost)
