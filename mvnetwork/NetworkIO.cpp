#include <boost/type_index.hpp>
#include <iostream>
#include <memory>

#include "NetworkIO.h"

namespace mvnetwork {

  NetworkIO::NetworkIO(std::unique_ptr<NetworkInterface>&& ni) {
    soc_ = std::make_unique<Socket>(std::move(ni));
  }

  NetworkIO::NetworkIO(const int fd) {
    soc_ = std::make_unique<Socket>(fd);
  }

  void NetworkIO::wait_for_accept(const std::function<void(const int fd)>& fn) {
    soc_->wait_for_accept(fn);
  }

  void NetworkIO::wait_for_read(const std::function<void()>& fn) {
    soc_->wait_for_read(fn);
  }

} // namespace mvnetwork
