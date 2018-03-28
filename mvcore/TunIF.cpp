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

#include "TunIF.h"

namespace mvcore {

  class TunIF::TunImpl {
  public:
    TunImpl(u_int32_t unit);
    int open_sock();
    void close_sock();
  private:
    u_int32_t unit;
    int fd;
  };

  TunIF::TunImpl::TunImpl(u_int32_t unit) : unit(unit) {}

  int TunIF::TunImpl::open_sock() {

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

    // set the fd non-blocking
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

    seteuid(0);

    if (connect(fd, (struct sockaddr *)&sc, sizeof(sc)) == -1) {
      std::cerr << "Can't create a tun interface." << std::endl;
      close(fd);
      return -1;
    }

    seteuid(getuid());

    std::cout << "A tun interface has been created." << std::endl;

    this->fd = fd;

    return 0;
  }

  void TunIF::TunImpl::close_sock() {
    close(this->fd);
  }

  TunIF::TunIF(u_int32_t unit): tun{std::make_unique<TunIF::TunImpl>(unit)} {}

  void TunIF::start() {
    auto tun_ptr = this->tun.get();
    tun_ptr->open_sock();

    system("ifconfig utun10 inet 10.0.7.1 10.0.7.1 mtu 1500 up");

    std::cout << "TunIF has started." << std::endl;
  }

  void TunIF::stop() {
    auto tun_ptr = this->tun.get();
    tun_ptr->close_sock();
    std::cout << "TunIF has stopped." << std::endl;
  }

}
