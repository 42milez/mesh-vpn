# https://stackoverflow.com/questions/42835837/how-to-clone-specific-submodules-from-boost-using-cmake

set(BOOST_GIT_TAG boost-1.65.1)

set(BOOST_ROOT ${CMAKE_BINARY_DIR})

find_package(Boost)

if (NOT Boost_FOUND)

  set(configure_cmd ./bootstrap.sh --prefix=${CMAKE_BINARY_DIR}
                                   --includedir=${CMAKE_BINARY_DIR}/include
                                   --libdir=${CMAKE_BINARY_DIR}/lib)

  set(build_cmd ./b2 headers)

  set(install_cmd ./b2 -j2 install)

  ExternalProject_Add(boost
    GIT_REPOSITORY    https://github.com/boostorg/boost.git
    GIT_TAG           ${BOOST_GIT_TAG}
    BUILD_IN_SOURCE   1
    CONFIGURE_COMMAND ${configure_cmd}
    BUILD_COMMAND     ${build_cmd}
    INSTALL_COMMAND   ${install_cmd})

  add_dependencies(boost openssl)

else (Boost_FOUND)

  add_custom_target(boost)

endif ()
