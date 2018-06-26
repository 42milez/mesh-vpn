#include <iostream>
#include <utility>

#include "Listener.h"

namespace mvcore {

  Listener::Listener(mvcore::NetTable *nettable) : nettable_(nettable) {
    logger_ = spdlog::stdout_color_mt("listener");
  }

  void Listener::start() {
    auto ni = std::make_unique<mvnetwork::NetworkInterface>();
    ni->port = 8888;
    listen_ = std::make_unique<mvnetwork::NetworkIO>(std::forward<decltype(ni)>(ni));
    worker_ = std::make_unique<Worker>();
    worker_->assign([this]{
      listen_->wait([this](const int soc){
        this->logger_->info("Connection request has arrived.");
        this->nettable_->add_remote_node(soc);
      });
    });
    worker_->start();
  }

  void Listener::stop() {
    worker_->stop();
    logger_->info("stop");
  }

} // mvcore
