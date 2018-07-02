#include <memory>

#include "RemoteNode.h"

namespace mvcore {

  RemoteNode::RemoteNode(const int fd) {
    soc_ = std::make_unique<mvnetwork::Socket>(fd);
    logger_ = spdlog::stdout_color_mt("remote_node");
    assign([this]{
      soc_->wait_for_read([this](){
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
