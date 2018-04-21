#include <iostream>
#include "Listener.h"

namespace mvcore {

  void Listener::start() {
    this->worker = std::make_unique<Worker>();
    this->worker->assign([]{
      std::cout << "worker is running..." << std::endl;
    });
    this->worker->start();
    std::cout << "[INFO] Lister has started." << std::endl;
  }

  void Listener::stop() {
    this->worker->stop();
    std::cout << "[INFO] Listener has stopped." << std::endl;
  }

}
