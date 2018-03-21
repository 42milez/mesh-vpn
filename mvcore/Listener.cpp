#include <iostream>
#include "Listener.h"

namespace mvcore {

  void Listener::start() {
    std::cout << "Lister has started." << std::endl;
  }

  void Listener::stop() {
    std::cout << "Listener has stopped." << std::endl;
  }

}
