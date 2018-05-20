#ifndef MESH_VPN_LISTENER_H
#define MESH_VPN_LISTENER_H

#include "NetTable.h"
#include "Service.h"
#include "Worker.h"
#include "mvnetwork/NetworkIO.h"

namespace mvcore {

  class Listener : public Service {
  public:
    explicit Listener(NetTable* nettable);
    void start() override;
    void stop() override;
  private:
    std::unique_ptr<Worker> worker;
    std::unique_ptr<mvnetwork::NetworkIO> listen;
    NetTable *nettable;
  };

} // namespace: mvcore

#endif // MESH_VPN_LISTENER_H
