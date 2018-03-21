#ifndef MESH_VPN_COMMON_H
#define MESH_VPN_COMMON_H

// Quote a given token stream to turn it into a string.
#define DEV_QUOTED_HELPER(s) #s
#define DEV_QUOTED(s) DEV_QUOTED_HELPER(s)

namespace mvcore {
  extern char const *Version;
}

#endif // MESH_VPN_COMMON_H
