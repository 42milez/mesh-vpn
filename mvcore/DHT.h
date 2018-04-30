#ifndef MESH_VPN_DHT_H
#define MESH_VPN_DHT_H

#include "Service.h"
#include "Worker.h"

namespace mvcore {

  class DHT : public Service {
  public:
    void start() override;
    void stop() override;
  private:
    std::unique_ptr<Worker> worker;
  };

} // namespace: mvcore

#endif // MESH_VPN_DHT_H
