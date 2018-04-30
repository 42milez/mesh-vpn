#include "LocalNode.h"
#include "DHT.h"
#include "Listener.h"
#include "NetTable.h"
#include "TunIF.h"

namespace mvcore {

  LocalNode::LocalNode() {
    auto nettable = new NetTable();
    auto dht = new DHT();
    auto listener = new Listener(nettable);
    //auto tunif = new TunIF();
    this->services[ServiceIdentifier::dht] = dht;
    this->services[ServiceIdentifier::listener] = listener;
    this->services[ServiceIdentifier::nettable] = nettable;
    //this->services[ServiceIdentifier::tunif] = tunif;
  }

  void LocalNode::start() {
    for (auto service : this->services) {
      service.second->start();
    }
  }

  void LocalNode::stop() {
    for (auto service : this->services) {
      service.second->stop();
    }
  }

}
