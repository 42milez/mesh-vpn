#include <iostream>

#include "NetTable.h"

namespace mvcore {

  NetTable::NetTable() {
    logger_ = spdlog::stdout_color_mt("nettable");
  }

  void NetTable::start() {}

  void NetTable::stop() {}

  void NetTable::add_remote_node(const int fd) {
    unique_lock lk = get_lock();
    cv_.wait(lk, [this, fd]{
      this->remote_nodes_.emplace_back(fd);
      this->logger_->info("Remote node has been added.");
      return true;
    });
    cv_.notify_all();
  }

  std::unique_lock<std::mutex> NetTable::get_lock() {
    return std::unique_lock<std::mutex>(mtx_);
  }

}
