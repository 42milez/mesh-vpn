#include <iostream>
#include "Listener.h"

namespace mvcore {

  void Listener::start() {
    std::cout << "[INFO] Lister has started." << std::endl;
  }

  void Listener::stop() {
    std::cout << "[INFO] Listener has stopped." << std::endl;
  }

}
