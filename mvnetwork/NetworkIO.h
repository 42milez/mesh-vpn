#ifndef MESH_VPN_NETWORKIO_H
#define MESH_VPN_NETWORKIO_H

#include <memory>

#include "Socket.h"

namespace mvnetwork {

  class NetworkIO {
  public:
    NetworkIO(int port);
    void wait(std::function<void(int soc)> fn);
  private:
    void initialize(int port);
    std::unique_ptr<Socket> soc;
  };

} // namespace mvnetwork

#endif //MESH_VPN_NETWORKIO_H
