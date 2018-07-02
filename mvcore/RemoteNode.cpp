#include <memory>

#include "RemoteNode.h"

namespace mvcore {

  RemoteNode::RemoteNode(const int fd) {
    listener_ = std::make_unique<mvnetwork::NetworkIO>(fd);
    logger_ = spdlog::stdout_color_mt("remote_node");
    assign([this]{
      listener_->wait_for_read([this](){
        this->logger_->info("received a message");
      });
    });
    start();
  }

  RemoteNode::~RemoteNode() {
    stop();
  }

  void RemoteNode::start() {
    run();
  }

  void RemoteNode::stop() {
    terminate();
  }

} // namespace mvcore
