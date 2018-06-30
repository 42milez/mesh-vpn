#ifndef MESH_VPN_SOCKET_H
#define MESH_VPN_SOCKET_H

#include <functional>

#include "NetworkInterface.h"

namespace mvnetwork {

  class Socket {
  public:
    Socket(int fd);
    Socket(std::unique_ptr<NetworkInterface> ni);
    Socket(const Socket& soc) = default;
    ~Socket();
    void wait(std::function<void(const int fd)> fn);
  private:
    void create_socket(int port);
    void create_multiplexer();
    int fd_;
    int mux_;
  };

} // mvnetwork

#endif //MESH_VPN_SOCKET_H
