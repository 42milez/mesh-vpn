#ifndef MESH_VPN_REMOTENODE_H
#define MESH_VPN_REMOTENODE_H

#include "mvnetwork/Socket.h"

namespace mvcore {

  class RemoteNode {
  public:
    // http://exlight.net/devel/cpp/explicit.html
    explicit RemoteNode(const int fd);
//    RemoteNode(RemoteNode&& rn) = default;
  private:
    std::unique_ptr<mvnetwork::Socket> soc_;
  };

} // namespace: mvcore

#endif // MESH_VPN_REMOTENODE_H
