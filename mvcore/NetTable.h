#ifndef MESH_VPN_NETTABLE_H
#define MESH_VPN_NETTABLE_H

#include "spdlog/spdlog.h"

#include <condition_variable>
#include <list>
#include <mutex>

#include "mvnetwork/Socket.h"

#include "RemoteNode.h"
#include "Service2.h"

namespace mvcore {

  class NetTable : public Service2 {
  public:
    NetTable();
    void add_remote_node(int fd);
    void start() override;
    void stop() override;
  private:
    using unique_lock = std::unique_lock<std::mutex>;
    std::unique_lock<std::mutex> get_lock();
    std::condition_variable cv_;
    std::shared_ptr<spdlog::logger> logger_;
    std::mutex mtx_;
    std::list<RemoteNode> remote_nodes_;
  };

} // namespace mvcore

#endif //MESH_VPN_NETTABLE_H
