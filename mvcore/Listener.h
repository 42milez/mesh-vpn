#ifndef MESH_VPN_LISTENER_H
#define MESH_VPN_LISTENER_H

#include "spdlog/spdlog.h"

#include "mvnetwork/NetworkIO.h"
#include "NetTable.h"
#include "Service.h"

namespace mvcore {

  class Listener : public Service {
  public:
    explicit Listener(NetTable* nettable);
    ~Listener() override;
    void start() override;
    void stop() override;
  private:
    NetTable *nettable_;
    std::unique_ptr<mvnetwork::NetworkIO> listener_;
    std::shared_ptr<spdlog::logger> logger_;
  };

} // namespace: mvcore

#endif // MESH_VPN_LISTENER_H
