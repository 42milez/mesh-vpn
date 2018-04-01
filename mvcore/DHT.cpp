#include <iostream>
#include "DHT.h"

namespace mvcore {

  void DHT::start() {
    std::cout << "[INFO] DHT has started." << std::endl;
  }

  void DHT::stop() {
    std::cout << "[INFO] DHT has stopped." << std::endl;
  }

}
