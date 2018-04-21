#include "Worker.h"

namespace mvcore {

  Worker::Worker() {}

  Worker::~Worker() {
    stop();
  }

  void Worker::assign(std::function<void()> fn) {
    this->fn = std::move(fn);
  }

  void Worker::start() {
    if (this->fn) {
      this->thread = new std::thread(this->fn);
    }
  }

  void Worker::stop() {
    if (this->thread && this->thread->joinable()) {
      this->thread->join();
      delete this->thread;
      this->thread = nullptr;
    }
  }

  void Worker::run() {}

}
