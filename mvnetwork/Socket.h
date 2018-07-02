#ifndef MESH_VPN_SOCKET_H
#define MESH_VPN_SOCKET_H

#include <functional>

#include "spdlog/spdlog.h"

#include "NetworkInterface.h"

namespace mvnetwork {

  class Socket {
  public:
    Socket(int fd);
    Socket(std::unique_ptr<NetworkInterface> ni);
    Socket(const Socket& soc) = default;
    ~Socket();
    void wait_for_accept(const std::function<void(const int fd)>& fn);
    void wait_for_read(const std::function<void()>& fn);
  private:
    void create_socket(int port);
    void create_multiplexer();
    int fd_;
    int mux_;
    std::shared_ptr<spdlog::logger> logger_;
  };

} // mvnetwork

#endif //MESH_VPN_SOCKET_H
