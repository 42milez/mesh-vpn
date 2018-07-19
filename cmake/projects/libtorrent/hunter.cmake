# !!! DO NOT PLACE HEADER GUARDS HERE !!!

include(hunter_add_version)
include(hunter_download)
include(hunter_pick_scheme)

hunter_add_version(
  PACKAGE_NAME
    libtorrent
  VERSION
    1.1.8
  URL
    https://github.com/arvidn/libtorrent/archive/libtorrent-1_1_8.tar.gz
  SHA1
    73fd5dce0d0daaf273fc5b06b0534f0fa2e64f0a
)

hunter_pick_scheme(DEFAULT url_sha1_libtorrent) # Use new custom scheme

hunter_download(PACKAGE_NAME libtorrent)
