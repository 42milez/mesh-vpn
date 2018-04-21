#ifndef MESH_VPN_LISTENER_H
#define MESH_VPN_LISTENER_H

#include "Service.h"
#include "Worker.h"

namespace mvcore {

  class Listener : public Service {
  public:
    void start();
    void stop();
  private:
    std::unique_ptr<Worker> worker;
  };

} // namespace: mvcore

#endif // MESH_VPN_LISTENER_H
