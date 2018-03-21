#ifndef MESH_VPN_LISTENER_H
#define MESH_VPN_LISTENER_H

#include "Service.h"

namespace mvcore {

  class Listener : public Service {
  public:
    void start();
    void stop();
  };

} // namespace: mvcore

#endif // MESH_VPN_LISTENER_H
