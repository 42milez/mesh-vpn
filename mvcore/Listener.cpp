#include <iostream>

#include "Listener.h"

namespace mvcore {

  Listener::Listener(mvcore::NetTable *nettable) : nettable(nettable) {}

  void Listener::start() {
    this->listen = std::make_unique<mvnetwork::NetworkIO>(8888);
    this->worker = std::make_unique<Worker>();
    this->worker->assign([this]{
      std::cout << "worker is running..." << std::endl;
      this->listen->wait([this](const int soc){
        std::cout << "Connection request has arrived." << std::endl;
        this->nettable->add_remote_node(soc);
      });
    });
    this->worker->start();
    std::cout << "[INFO] Lister has started." << std::endl;
  }

  void Listener::stop() {
    this->worker->stop();
    std::cout << "[INFO] Listener has stopped." << std::endl;
  }

} // mvcore
