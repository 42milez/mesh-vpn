#include <iostream>
#include <utility>

#include "Listener.h"

namespace mvcore {

  Listener::Listener(mvcore::NetTable *nettable) : nettable_(nettable) {
    auto ni = std::make_unique<mvnetwork::NetworkInterface>();
    ni->port = 8888;
    listener_ = std::make_unique<mvnetwork::NetworkIO>(std::move(ni));
    logger_ = spdlog::stdout_color_mt("listener");
    assign([this]{
      listener_->wait_for_accept([this](const int fd){
        this->logger_->info("Connection request has arrived.");
        this->nettable_->add_remote_node(fd);
      });
    });
  }

  Listener::~Listener() {
    stop();
  }

  void Listener::start() {
    run();
  }

  void Listener::stop() {
    terminate();
  }

} // mvcore
