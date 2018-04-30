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
    run();
  }

  void Worker::stop() {
    if (this->thread && this->thread->joinable()) {
      this->is_abort = true;
      this->thread->join();
      delete this->thread;
      this->thread = nullptr;
    }
  }

  void Worker::run() {
    if (this->fn) {
      this->thread = new std::thread([&fn = this->fn, &is_abort = this->is_abort]{
        for (;;) {
          if (is_abort) break;
          fn();
          std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
      });
    }
  }

}
