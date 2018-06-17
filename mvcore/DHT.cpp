#include <iostream>

#include "DHT.h"

namespace mvcore {

  DHT::DHT() {
    logger_ = spdlog::stdout_color_mt("dht");
  }

  void DHT::start() {
    worker_ = std::make_unique<Worker>();
    worker_->assign([this]{
      this->logger_->info("running...");
    });
    worker_->start();
  }

  void DHT::stop() {
    worker_->stop();
    logger_->info("stop");
  }

} // namespace: mvcore
