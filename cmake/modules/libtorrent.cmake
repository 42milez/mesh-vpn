include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/libtorrent-rasterbar/libtorrent-rasterbarConfigVersion.cmake"
  VERSION 1.1.5
  COMPATIBILITY ExactVersion
)

set(ConfigPackageLocation ${CMAKE_BINARY_DIR}/libtorrent-rasterbar)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/libtorrent-rasterbar/libtorrent-rasterbarConfigVersion.cmake"
  DESTINATION
    ${ConfigPackageLocation}
)

set(configure_cmd ./bootstrap.sh --prefix=${CMAKE_BINARY_DIR}
                                 --includedir=${CMAKE_BINARY_DIR}/include
                                 --libdir=${CMAKE_BINARY_DIR}/lib
                                 --with-boost=${CMAKE_BINARY_DIR}
                                 --with-openssl=${CMAKE_BINARY_DIR})

set(build_cmd make -j2)

set(install_cmd make install)

ExternalProject_Add(libtorrent
  GIT_REPOSITORY    https://github.com/arvidn/libtorrent.git
  GIT_TAG           libtorrent-1_1_5
  BUILD_IN_SOURCE   1
  CONFIGURE_COMMAND ${configure_cmd}
  BUILD_COMMAND     ${build_cmd}
  INSTALL_COMMAND   ${install_cmd})

add_dependencies(libtorrent openssl boost)
