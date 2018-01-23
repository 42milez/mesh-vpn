set(LibtorrentRasterbar_INCLUDE_DIR ${CMAKE_BINARY_DIR}/include/libtorrent)
set(LibtorrentRasterbar_LIBRARY ${CMAKE_BINARY_DIR}/lib)
set(LIBTORRENT_GIT_TAG libtorrent-1_1_5)

find_package(PkgConfig QUIET)

if(PKG_CONFIG_FOUND)
  pkg_check_modules(PC_LIBTORRENT_RASTERBAR QUIET libtorrent-rasterbar)
endif()

if(LibtorrentRasterbar_USE_STATIC_LIBS)
  set(LibtorrentRasterbar_ORIG_CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})
  set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
endif()

if(PC_LIBTORRENT_RASTERBAR_FOUND)
  set(LibtorrentRasterbar_DEFINITIONS ${PC_LIBTORRENT_RASTERBAR_CFLAGS})
else()
  if(LibtorrentRasterbar_CUSTOM_DEFINITIONS)
    set(LibtorrentRasterbar_DEFINITIONS ${LibtorrentRasterbar_CUSTOM_DEFINITIONS})
  else()
    # Without pkg-config, we can't possibly figure out the correct build flags.
    # libtorrent is very picky about those. Let's take a set of defaults and
    # hope that they apply. If not, you the user are on your own.
    set(LibtorrentRasterbar_DEFINITIONS
      -DTORRENT_USE_OPENSSL
      -DTORRENT_DISABLE_GEO_IP
      -DBOOST_ASIO_ENABLE_CANCELIO
      -DUNICODE -D_UNICODE -D_FILE_OFFSET_BITS=64)
  endif()

  if(LibtorrentRasterbar_USE_STATIC_LIBS)
    list(APPEND LibtorrentRasterbar_DEFINITIONS -DBOOST_ASIO_SEPARATE_COMPILATION)
  else()
    list(APPEND LibtorrentRasterbar_DEFINITIONS
      -DTORRENT_LINKING_SHARED -DBOOST_ASIO_DYN_LINK
      -DBOOST_DATE_TIME_DYN_LINK -DBOOST_THREAD_DYN_LINK
      -DBOOST_SYSTEM_DYN_LINK -DBOOST_CHRONO_DYN_LINK)
  endif()
endif()

message(STATUS "libtorrent definitions: ${LibtorrentRasterbar_DEFINITIONS}")

find_path(LibtorrentRasterbar_INCLUDE_DIR libtorrent
  HINTS ${PC_LIBTORRENT_RASTERBAR_INCLUDEDIR} ${PC_LIBTORRENT_RASTERBAR_INCLUDE_DIRS}
  PATH_SUFFIXES libtorrent-rasterbar)

find_library(LibtorrentRasterbar_LIBRARY NAMES torrent-rasterbar
  HINTS ${PC_LIBTORRENT_RASTERBAR_LIBDIR} ${PC_LIBTORRENT_RASTERBAR_LIBRARY_DIRS})

if(LibtorrentRasterbar_USE_STATIC_LIBS)
  set(CMAKE_FIND_LIBRARY_SUFFIXES ${LibtorrentRasterbar_ORIG_CMAKE_FIND_LIBRARY_SUFFIXES})
endif()

set(LibtorrentRasterbar_LIBRARIES ${LibtorrentRasterbar_LIBRARY})
set(LibtorrentRasterbar_INCLUDE_DIRS ${LibtorrentRasterbar_INCLUDE_DIR})

if(NOT Boost_SYSTEM_FOUND OR NOT Boost_THREAD_FOUND OR NOT Boost_DATE_TIME_FOUND OR NOT Boost_CHRONO_FOUND)
  find_package(Boost REQUIRED COMPONENTS system thread date_time chrono)
  set(LibtorrentRasterbar_LIBRARIES ${LibtorrentRasterbar_LIBRARIES} ${Boost_LIBRARIES})
  set(LibtorrentRasterbar_INCLUDE_DIRS ${LibtorrentRasterbar_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
endif()

list(FIND LibtorrentRasterbar_DEFINITIONS -DTORRENT_USE_OPENSSL LibtorrentRasterbar_ENCRYPTION_INDEX)
if(LibtorrentRasterbar_ENCRYPTION_INDEX GREATER -1)
  find_package(OpenSSL REQUIRED)
  set(LibtorrentRasterbar_LIBRARIES ${LibtorrentRasterbar_LIBRARIES} ${OPENSSL_LIBRARIES})
  set(LibtorrentRasterbar_INCLUDE_DIRS ${LibtorrentRasterbar_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIRS})
  set(LibtorrentRasterbar_OPENSSL_ENABLED ON)
endif()

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LibtorrentRasterbar_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LibtorrentRasterbar DEFAULT_MSG
  LibtorrentRasterbar_LIBRARY
  LibtorrentRasterbar_INCLUDE_DIR
  Boost_SYSTEM_FOUND
  Boost_THREAD_FOUND
  Boost_DATE_TIME_FOUND
  Boost_CHRONO_FOUND)

mark_as_advanced(LibtorrentRasterbar_INCLUDE_DIR LibtorrentRasterbar_LIBRARY
  LibtorrentRasterbar_ORIG_CMAKE_FIND_LIBRARY_SUFFIXES
  LibtorrentRasterbar_ENCRYPTION_INDEX)

if (NOT LibtorrentRasterbar_FOUND)

  set(configure_cmd ./bootstrap.sh --prefix=${CMAKE_BINARY_DIR}
                                   --includedir=${CMAKE_BINARY_DIR}/include
                                   --libdir=${CMAKE_BINARY_DIR}/lib
                                   --with-boost=${CMAKE_BINARY_DIR}
                                   --with-openssl=${CMAKE_BINARY_DIR})

  set(build_cmd make -j2)

  set(install_cmd make install)

  ExternalProject_Add(libtorrent
    GIT_REPOSITORY    https://github.com/arvidn/libtorrent.git
    GIT_TAG           ${LIBTORRENT_GIT_TAG}
    BUILD_IN_SOURCE   1
    CONFIGURE_COMMAND ${configure_cmd}
    BUILD_COMMAND     ${build_cmd}
    INSTALL_COMMAND   ${install_cmd})

  add_dependencies(libtorrent openssl boost)

else (LibtorrentRasterbar_FOUND)

  add_custom_target(libtorrent)

endif ()
