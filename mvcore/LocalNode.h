#ifndef MESH_VPN_LOCALNODE_H
#define MESH_VPN_LOCALNODE_H

#include <vector>

#include "Service.h"

namespace mvcore {
  class LocalNode {
  public:
    void start();
    void stop();
  private:
    std::vector<Service> services;
  };
}

#endif //MESH_VPN_LOCALNODE_H
