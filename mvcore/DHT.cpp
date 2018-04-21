#include <iostream>

#include "DHT.h"

namespace mvcore {

  void DHT::start() {
    this->worker = std::make_unique<Worker>();
    this->worker->assign([]{
      std::cout << "worker is running..." << std::endl;
    });
    this->worker->start();
    std::cout << "[INFO] DHT has started." << std::endl;
  }

  void DHT::stop() {
    this->worker->stop();
    std::cout << "[INFO] DHT has stopped." << std::endl;
  }

} // namespace: mvcore
