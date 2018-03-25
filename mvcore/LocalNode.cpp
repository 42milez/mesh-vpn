#include "LocalNode.h"
#include "mvcore/DHT.h"
#include "mvcore/Listener.h"
#include "mvcore/TunIF.h"

namespace mvcore {

  LocalNode::LocalNode() {
    this->services.emplace_back(new TunIF(static_cast<u_int32_t >(strtol("10.0.7.1", nullptr, 10) + 1)));
    this->services.emplace_back(new Listener());
    this->services.emplace_back(new DHT());
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
