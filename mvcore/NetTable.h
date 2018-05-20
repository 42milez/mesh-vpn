#ifndef MESH_VPN_NETTABLE_H
#define MESH_VPN_NETTABLE_H

#include <condition_variable>
#include <list>
#include <mutex>

#include "RemoteNode.h"
#include "Service.h"

namespace mvcore {

  class NetTable : public Service {
  public:
    void add_remote_node(int soc);
    void start() override;
    void stop() override;
  private:
    using unique_lock = std::unique_lock<std::mutex>;
    std::unique_lock<std::mutex> get_lock();
    std::mutex mtx;
    std::condition_variable cv;
    std::list<RemoteNode> remote_nodes;
  };

} // namespace mvcore

#endif //MESH_VPN_NETTABLE_H
