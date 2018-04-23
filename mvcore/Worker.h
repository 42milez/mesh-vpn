#ifndef MESH_VPN_WORKER_H
#define MESH_VPN_WORKER_H

#include <thread>

namespace mvcore {

  class Worker {
  public:
    Worker();
    ~Worker();
    void assign(std::function<void()> fn);
    void start();
    void stop();
    Worker(const Worker &) = delete;
    void operator=(const Worker &) = delete;
  private:
    // https://stackoverflow.com/questions/12797051/implicit-instantiation-of-undefined-template-when-forward-declaring-template-c
    std::function<void()> fn;
    std::thread *thread = nullptr;
    void run();
  };

};

#endif //MESH_VPN_WORKER_H
