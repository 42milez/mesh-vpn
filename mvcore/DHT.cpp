#include <iostream>

#include "DHT.h"

namespace mvcore {

  DHT::DHT() {
    logger_ = spdlog::stdout_color_mt("dht");
    assign([this]{
      this->logger_->info("running...");
    });
  }

  DHT::~DHT() {
    stop();
  }

  void DHT::start() {
    run();
  }

  void DHT::stop() {
    terminate();
  }

} // namespace: mvcore
