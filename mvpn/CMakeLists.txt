set(SOURCE_FILES mvpn.cpp)

add_executable(mvpn ${SOURCE_FILES})

target_link_libraries(mvpn
  PUBLIC
    OpenSSL::SSL
    OpenSSL::Crypto
    Boost::boost
    Boost::system
    ${HUNTER_INSTALL_PREFIX}/lib/libtorrent-rasterbar.a
  PRIVATE
    mvcore
    mvcrypt
    mvnetwork
    mvutil
)
