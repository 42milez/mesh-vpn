#ifndef MESH_VPN_SERVICE_H
#define MESH_VPN_SERVICE_H

namespace mvcore {

  class Service {
  public:
    virtual ~Service() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
  };

} // namespace: mvcore

#endif // MESH_VPN_SERVICE_H
