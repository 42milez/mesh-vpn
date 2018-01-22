set(SOURCE_FILES cli.cpp)

add_executable(cli ${SOURCE_FILES})

add_dependencies(cli libtorrent)

set(openssl_libs
  ${CMAKE_BINARY_DIR}/lib/libcrypto.a
  ${CMAKE_BINARY_DIR}/lib/libssl.a
)

file(GLOB boost_libs "${CMAKE_BINARY_DIR}/lib/libboost_*.a")

set(libtorrent_libs
  ${CMAKE_BINARY_DIR}/lib/libtorrent-rasterbar.a
)

include_directories(${CMAKE_BINARY_DIR}/include/openssl)
include_directories(${CMAKE_BINARY_DIR}/include/boost)

target_link_libraries(cli
  PRIVATE
  mvcore
  mvcrypt
  mvutil
  ${openssl_libs}
  ${boost_libs}
  ${libtorrent_libs}
)
