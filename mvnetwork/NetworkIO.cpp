#include <iostream>

#include "NetworkIO.h"

namespace mvnetwork {

  NetworkIO::NetworkIO(const int port) {
    initialize(port);
  }

  void NetworkIO::initialize(const int port) {
    this->soc = std::make_unique<Socket>(port);
  }

  void NetworkIO::wait(std::function<void(const int soc)> fn) {
    this->soc->wait(std::move(fn));
  }

} // namespace mvnetwork
