#ifndef MESH_VPN_SERVICE2_H
#define MESH_VPN_SERVICE2_H

#include <memory>
#include <thread>

namespace mvcore {

  class Service2 {
  public:
    virtual ~Service2() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
  protected:
    void assign(std::function<void()> fn);
    void run();
    void terminate();
    // https://stackoverflow.com/questions/12797051/implicit-instantiation-of-undefined-template-when-forward-declaring-template-c
    std::function<void()> fn_;
    bool is_abort_ { false };
    std::unique_ptr<std::thread> thread_;
  };

}

#endif //MESH_VPN_SERVICE2_H
