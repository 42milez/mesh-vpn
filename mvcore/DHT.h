#ifndef MESH_VPN_DHT_H
#define MESH_VPN_DHT_H

#include "spdlog/spdlog.h"

#include "Service.h"
#include "Worker.h"

namespace mvcore {

  class DHT : public Service {
  public:
    DHT();
    ~DHT() override;
    void start() override;
    void stop() override;
  private:
    std::shared_ptr<spdlog::logger> logger_;
  };

} // namespace: mvcore

#endif // MESH_VPN_DHT_H
