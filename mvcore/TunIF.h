#ifndef MESH_VPN_TUNIF_H
#define MESH_VPN_TUNIF_H

#include "Service.h"

namespace mvcore {

  class TunIF: public Service {
  public:
    TunIF(u_int32_t unit);
    void start() override;
    void stop() override;
  private:
    class TunImpl;
    std::unique_ptr<TunImpl> tun;
    int sock;
  };

} // namespace: mvcore


#endif // MESH_VPN_TUNIF_H
