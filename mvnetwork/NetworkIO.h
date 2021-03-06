#ifndef MESH_VPN_NETWORKIO_H
#define MESH_VPN_NETWORKIO_H

#include <memory>

#include "NetworkInterface.h"
#include "Socket.h"

namespace mvnetwork {

  class NetworkIO {
  public:
    NetworkIO(std::unique_ptr<NetworkInterface>&& ni);
    NetworkIO(int fd);
    void wait_for_accept(const std::function<void(const int fd)>& fn);
    void wait_for_read(const std::function<void()>& fn);
  private:
    std::unique_ptr<Socket> soc_;
  };

} // namespace mvnetwork

#endif //MESH_VPN_NETWORKIO_H
