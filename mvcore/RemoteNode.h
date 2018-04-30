#ifndef MESH_VPN_REMOTENODE_H
#define MESH_VPN_REMOTENODE_H

namespace mvcore {

  class RemoteNode {
  public:
    // http://exlight.net/devel/cpp/explicit.html
    explicit RemoteNode(int soc);
    ~RemoteNode();
  private:
    int soc;
  };

} // namespace: mvcore

#endif // MESH_VPN_REMOTENODE_H
