#include <boost/type_index.hpp>
#include <iostream>
#include <memory>

#include "NetworkIO.h"

namespace mvnetwork {

  NetworkIO::NetworkIO(std::unique_ptr<NetworkInterface>&& ni) {
    soc_ = std::make_unique<Socket>(std::move(ni));
  }

  void NetworkIO::wait(std::function<void(const int fd)> fn) {
    soc_->wait_for_accept(fn);
  }

} // namespace mvnetwork
