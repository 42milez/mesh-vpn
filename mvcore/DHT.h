#ifndef MESH_VPN_DHT_H
#define MESH_VPN_DHT_H

#include "Service.h"

namespace mvcore {
  class DHT : public Service {
  public:
    void start();
    void stop();
  };
}

#endif // MESH_VPN_DHT_H
