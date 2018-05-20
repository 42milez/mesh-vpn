#ifndef MESH_VPN_LOG_H
#define MESH_VPN_LOG_H

#include <string>

namespace mvutil {

  class Log {
  public:
    void debug();
    void info();
    void warn();
  };

} // namespace mvutil

#endif //MESH_VPN_LOG_H
