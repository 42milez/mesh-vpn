#ifndef MESH_VPN_REMOTENODE_H
#define MESH_VPN_REMOTENODE_H

#include "spdlog/spdlog.h"

#include "mvcore/Service.h"
#include "mvnetwork/Socket.h"

namespace mvcore {

  class RemoteNode : public Service {
  public:
    void start() override;
    void stop() override;
    // http://exlight.net/devel/cpp/explicit.html
    explicit RemoteNode(int fd);
    ~RemoteNode();
  private:
    std::unique_ptr<mvnetwork::Socket> soc_;
    std::shared_ptr<spdlog::logger> logger_;
  };

} // namespace: mvcore

#endif // MESH_VPN_REMOTENODE_H
