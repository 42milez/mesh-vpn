set(SOURCE_FILES mvpn.cpp)

add_executable(mvpn ${SOURCE_FILES})

add_dependencies(mvpn libtorrent)

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
include_directories(${CMAKE_BINARY_DIR}/include/libtorrent)

target_link_libraries(mvpn
  PRIVATE
  mvcore
  mvcrypt
  mvnetwork
  mvutil
  ${openssl_libs}
  ${boost_libs}
  ${libtorrent_libs}
)
