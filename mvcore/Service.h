#ifndef MESH_VPN_SERVICE_H
#define MESH_VPN_SERVICE_H

namespace mvcore {
  class Service {
  public:
    virtual void start();
    virtual void stop();
  };
}


#endif //MESH_VPN_SERVICE_H
