#include <iostream>

#include "NetTable.h"

namespace mvcore {

  void NetTable::start() {

  }

  void NetTable::stop() {

  }

  void NetTable::add_remote_node(const int soc) {
    unique_lock lk = get_lock();
    cv.wait(lk, [this, soc]{
      this->remote_nodes.emplace_back(soc);
      std::cout << "Remote node has been added." << std::endl;
      return true;
    });
    cv.notify_all();
  }

  std::unique_lock<std::mutex> NetTable::get_lock() {
    return std::unique_lock<std::mutex>(this->mtx);
  }

}
