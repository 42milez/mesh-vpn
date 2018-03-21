#include <iostream>
#include "TunInterface.h"

namespace mvcore {

  void TunInterface::start() {
    std::cout << "TunInterface has started." << std::endl;
  }

  void TunInterface::stop() {
    std::cout << "TunInterface has stopped." << std::endl;
  }

}
