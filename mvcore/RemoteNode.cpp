#include <memory>
#include <unistd.h>

#include "RemoteNode.h"

namespace mvcore {

  RemoteNode::RemoteNode(const int fd) {
    soc_ = std::make_unique<mvnetwork::Socket>(fd);
  }

} // namespace mvcore
