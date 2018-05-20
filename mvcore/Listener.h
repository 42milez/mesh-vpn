#ifndef MESH_VPN_LISTENER_H
#define MESH_VPN_LISTENER_H

#include "spdlog-src/include/spdlog/spdlog.h"

#include "mvnetwork/NetworkIO.h"
#include "NetTable.h"
#include "Service.h"
#include "Worker.h"

namespace mvcore {

  class Listener : public Service {
  public:
    explicit Listener(NetTable* nettable);
    void start() override;
    void stop() override;
  private:
    std::unique_ptr<mvnetwork::NetworkIO> listen_;
    std::shared_ptr<spdlog::logger> logger_;
    NetTable *nettable_;
    std::unique_ptr<Worker> worker_;
  };

} // namespace: mvcore

#endif // MESH_VPN_LISTENER_H
