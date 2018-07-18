set(SOURCE_FILES mvpn.cpp)

add_executable(mvpn ${SOURCE_FILES})

target_link_libraries(mvpn
  PUBLIC
    Boost::boost
    Boost::system
    OpenSSL::SSL
    OpenSSL::Crypto
    ${HUNTER_INSTALL_PREFIX}/lib/libtorrent-rasterbar.a
  PRIVATE
    mvcore
    mvcrypt
    mvnetwork
    mvutil
)
