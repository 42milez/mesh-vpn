#ifndef MESH_VPN_LOCALNODE_H
#define MESH_VPN_LOCALNODE_H

#include <vector>

namespace mvcore {

  class Service;

  class LocalNode {
  public:
    LocalNode();
    void start();
    void stop();
  private:
    std::vector<Service*> services;
  };

} // namespace: mvcore

#endif // MESH_VPN_LOCALNODE_H
