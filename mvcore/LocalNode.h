#ifndef MESH_VPN_LOCALNODE_H
#define MESH_VPN_LOCALNODE_H

#include <map>
#include <string>

namespace mvcore {

  class Service;

  class LocalNode {
  public:
    LocalNode();
    void start();
    void stop();
  private:
    enum class ServiceIdentifier {
      dht,
      listener,
      nettable,
      tunif
    };
    std::map<ServiceIdentifier, Service*> services;
  };

} // namespace: mvcore

#endif // MESH_VPN_LOCALNODE_H
