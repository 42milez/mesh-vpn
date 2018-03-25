#include "LocalNode.h"
#include "mvcore/DHT.h"
#include "mvcore/Listener.h"
#include "mvcore/TunInterface.h"

namespace mvcore {

  LocalNode::LocalNode() {
    auto dht = new DHT();
    auto listener = new Listener();
    auto interface = new TunInterface(static_cast<u_int32_t >(strtol("10.0.7.1", nullptr, 10) + 1));

    this->services.emplace_back(dht);
    this->services.emplace_back(listener);
    this->services.emplace_back(interface);
  }

  void LocalNode::start() {
    for (auto service : this->services) {
      service->start();
    }
  }

  void LocalNode::stop() {
    for (auto service : this->services) {
      service->stop();
    }
  }

}
