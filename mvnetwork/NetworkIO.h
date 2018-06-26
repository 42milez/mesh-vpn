#ifndef MESH_VPN_NETWORKIO_H
#define MESH_VPN_NETWORKIO_H

#include <memory>

#include "NetworkInterface.h"
#include "Socket.h"

namespace mvnetwork {

  class NetworkIO {
  public:
    NetworkIO(std::unique_ptr<NetworkInterface>&& ni);
    NetworkIO(std::unique_ptr<Socket>&& soc);
    void wait(std::function<void(int soc)> fn);
  private:
    std::unique_ptr<Socket> create_socket(int port);
    std::unique_ptr<NetworkInterface> ni_;
    std::unique_ptr<Socket> soc_;
  };

} // namespace mvnetwork

#endif //MESH_VPN_NETWORKIO_H
