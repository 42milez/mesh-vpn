#include <iostream>
#include <sys/kern_control.h>
#include <netinet/in.h>
#include <net/if_utun.h>
#include <sys/ioctl.h>
#include <sys/kern_event.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <netdb.h>


#include "TunIF.h"

namespace mvcore {

  class TunIF::TunImpl {
  public:
    TunImpl(u_int32_t unit);
    int open_sock();
    void close_sock();
    void make_utun(const std::string& ifname, const std::string& option);
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
      return -1;
    }

    if (ioctl(fd, CTLIOCGINFO, &ci) == -1) {
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
      close(fd);
      return -1;
    }

    this->fd = fd;

    return 0;
  }

  void TunIF::TunImpl::close_sock() {
    close(this->fd);
  }

  void TunIF::TunImpl::make_utun(const std::string& ifname, const std::string& option) {
    // http://man7.org/linux/man-pages/man3/getifaddrs.3.html
    struct ifaddrs *ifaddr, *ifa;
    int family, s, n;
    char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1) {
      return;
    }

    for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
      if (ifa->ifa_addr == NULL) {
        continue;
      }
      family = ifa->ifa_addr->sa_family;

      std::string af;
      if (family == AF_INET) af = "AF_INET";
      else if (family == AF_INET6) af = "AF_INET6";
      else af = family;

      std::cout << ifa->ifa_name << ", " << family << std::endl;
    }

    std::string cmd = "ifconfig";

    system((cmd + " " + ifname + " " + option).c_str());
  }

  TunIF::TunIF(u_int32_t unit): tun{std::make_unique<TunIF::TunImpl>(unit)} {
    std::cout << "sc_unit: " << unit << std::endl;
  }

  void TunIF::start() {
    auto tun_ptr = this->tun.get();

    if (tun_ptr->open_sock() == 0) {
      std::cout << "A tun interface has been created." << std::endl;
    } else {
      std::cerr << "Can't create a tun interface." << std::endl;
    }

    tun_ptr->make_utun("utun10", "inet 10.0.7.1 10.0.7.1 mtu 1500 up");
    std::cout << "TunIF has started." << std::endl;
  }

  void TunIF::stop() {
    auto tun_ptr = this->tun.get();
    tun_ptr->close_sock();
    std::cout << "TunIF has stopped." << std::endl;
  }

}
