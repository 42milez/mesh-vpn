#ifndef MESH_VPN_SOCKET_H
#define MESH_VPN_SOCKET_H

namespace mvnetwork {

  class Socket {
  public:
    Socket(int port);
    ~Socket();
    void wait(std::function<void(int soc)> fn);
  private:
    void initialize(int port);
    int mux;
    int soc;
  };

} // mvnetwork

#endif //MESH_VPN_SOCKET_H
