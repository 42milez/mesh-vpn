set(OPENSSL_ROOT_DIR ${CMAKE_BINARY_DIR})

find_package(OpenSSL)

if (NOT OPENSSL_FOUND)

  set(configure_cmd ./config --prefix=${CMAKE_BINARY_DIR}
                             --openssldir=${CMAKE_BINARY_DIR}/include/openssl
                             --libdir=lib)

  set(build_cmd make -j2)

  set(install_cmd make install)

  ExternalProject_Add(openssl
    GIT_REPOSITORY    https://github.com/openssl/openssl.git
    GIT_TAG           OpenSSL_1_1_0g
    BUILD_IN_SOURCE   1
    CONFIGURE_COMMAND ${configure_cmd}
    BUILD_COMMAND     ${build_cmd}
    INSTALL_COMMAND   ${install_cmd})

else (OPENSSL_FOUND)

  add_custom_target(openssl)

endif ()
