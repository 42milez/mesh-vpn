#include <unistd.h>

#include "RemoteNode.h"

namespace mvcore {

  RemoteNode::RemoteNode(const int soc) {
    this->soc = soc;
  }

  RemoteNode::~RemoteNode() {
    close(this->soc);
  }

} // namespace mvcore
