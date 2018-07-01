#include "Service2.h"

namespace mvcore {

  void Service2::assign(std::function<void()> fn) {
    fn_ = std::move(fn);
  }

  void Service2::run() {
    if (fn_) {
      thread_ = std::make_unique<std::thread>([&fn = fn_, &is_abort = is_abort_]{
        for (;;) {
          if (is_abort) break;
          fn();
          std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
      });
    }
  }

  void Service2::terminate() {
    if (thread_ && thread_->joinable()) {
      is_abort_ = true;
      thread_->join();
    }
  }

} // mvcore
