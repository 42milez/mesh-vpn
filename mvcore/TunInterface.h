#ifndef MESH_VPN_INTERFACE_H
#define MESH_VPN_INTERFACE_H

#include "Service.h"

namespace mvcore {

  class TunInterface: public Service {
  public:
    TunInterface(u_int32_t unit);
    void start() override;
    void stop() override;
  private:
    class TunImpl;
    std::unique_ptr<TunImpl> tun;
    int sock;
  };

} // namespace: mvcore


#endif // MESH_VPN_INTERFACE_H
