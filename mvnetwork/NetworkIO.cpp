#include <iostream>
#include <memory>

#include "NetworkIO.h"

namespace mvnetwork {

  NetworkIO::NetworkIO(std::unique_ptr<NetworkInterface>&& ni) {
    ni_ = std::move(ni);
    soc_ = std::move(create_socket(ni_->port));
  }

  NetworkIO::NetworkIO(std::unique_ptr<Socket>&& soc) {
    soc_ = std::move(soc);
  }

  std::unique_ptr<Socket> NetworkIO::create_socket(const int port) {
    return std::make_unique<Socket>(port);
  }

  void NetworkIO::wait(std::function<void(const int soc)> fn) {
    soc_->wait(std::move(fn));
  }

} // namespace mvnetwork
