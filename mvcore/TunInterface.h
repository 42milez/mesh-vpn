#ifndef MESH_VPN_INTERFACE_H
#define MESH_VPN_INTERFACE_H

#include "Service.h"

namespace mvcore {

  class TunInterface : public Service {
  public:
    void start();
    void stop();
  };

} // namespace: mvcore


#endif // MESH_VPN_INTERFACE_H
