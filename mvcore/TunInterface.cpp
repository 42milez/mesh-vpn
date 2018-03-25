#include <iostream>
#include <sys/kern_control.h>
#include <netinet/in.h>
#include <net/if_utun.h>
#include <sys/ioctl.h>
#include <sys/kern_event.h>
#include <unistd.h>
#include <fcntl.h>
//#include <arpa/inet.h>
//#include <net/if.h>
//#include <netinet/ip.h>
//#include <netinet/ip_icmp.h>
//#include <netinet/tcp.h>
//#include <netinet/udp.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>

#include "TunInterface.h"

namespace mvcore {

  class TunInterface::TunImpl {
  public:
    TunImpl(u_int32_t unit);
    int open_tun();
    void close_tun();
  private:
    u_int32_t unit;
  };

  TunInterface::TunImpl::TunImpl(u_int32_t unit) : unit(unit) {}

  int TunInterface::TunImpl::open_tun() {

    // Protocol families are mapped onto address families
    // --------------------------------------------------
    // Notes:
    //  - KEXT Controls and Notifications
    //    https://developer.apple.com/library/content/documentation/Darwin/Conceptual/NKEConceptual/control/control.html
    auto fd = socket(PF_SYSTEM, SOCK_DGRAM, SYSPROTO_CONTROL);
    if (fd == -1) {
      std::cerr << "socket(SYSPROTO_CONTROL)" << std::endl;
      return -1;
    }

    // set the socket non-blocking
    auto err = fcntl(fd, F_SETFL, O_NONBLOCK);
    if (err != 0) {
      close(fd);
      return err;
    }

    // set close-on-exec flag
    auto flags = fcntl(fd, F_GETFD);
    flags |= FD_CLOEXEC;
    err = fcntl(fd, F_SETFD, flags);
    if (err != 0) {
      close(fd);
      return err;
    }

    // Convert a kernel control name to a kernel control id
    // --------------------------------------------------
    // Notes:
    //  - CTLIOCGINFO
    //    https://developer.apple.com/documentation/kernel/ctliocginfo?language=objc
    //  - strlcpy
    //    https://en.wikipedia.org/wiki/C_string_handling
    ctl_info ci{};

    if (strlcpy(ci.ctl_name, UTUN_CONTROL_NAME, sizeof(ci.ctl_name)) >= sizeof(ci.ctl_name)) {
      std::cerr << "UTUN_CONTROL_NAME is too long" << std::endl;
      return -1;
    }

    if (ioctl(fd, CTLIOCGINFO, &ci) == -1) {
      std::cerr << "ioctl(CTLIOCGINFO)" << std::endl;
      close(fd);
      return -1;
    }

    sockaddr_ctl sc{};

    sc.sc_id      = ci.ctl_id;
    sc.sc_len     = sizeof(sc);
    sc.sc_family  = AF_SYSTEM;
    sc.ss_sysaddr = AF_SYS_CONTROL;
    sc.sc_unit    = this->unit;

    if (connect(fd, (struct sockaddr *)&sc, sizeof(sc)) == -1) {
      std::cerr << "connect(AF_SYS_CONTROL)" << std::endl;
      close(fd);
      return -1;
    }

    return fd;
  }

  void TunInterface::TunImpl::close_tun() {

  }

  TunInterface::TunInterface(u_int32_t unit): tun{std::make_unique<TunInterface::TunImpl>(unit)} {}

  void TunInterface::start() {
    auto tun_ptr = this->tun.get();
    this->sock = tun_ptr.open_tun();
    if (this->sock < 0) {
      std::cerr << "Can't create a tun interface." << std::endl;
    }

    std::cout << "A tun interface has been created." << std::endl;

    //execl("echo 'pass' | sudo -S ifconfig utun10 inet 10.0.7.1 10.0.7.1 mtu 1500 up");
    execl("/usr/bin/sudo", "/sbin/ifconfig", "utun10", "inet", "10.0.7.1", "10.0.7.1", "mtu", "1500", "up");

    std::cout << "TunInterface has started." << std::endl;
  }

  void TunInterface::stop() {
    close(this->sock);
    std::cout << "TunInterface has stopped." << std::endl;
  }

}
