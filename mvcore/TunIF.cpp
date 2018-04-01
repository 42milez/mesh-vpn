#include <iostream>
#include <sys/kern_control.h>
#include <netinet/in.h>
#include <net/if_utun.h>
#include <sys/ioctl.h>
#include <sys/kern_event.h>
#include <unistd.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <vector>
#include <algorithm>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>

#include "TunIF.h"

namespace mvcore {

  class TunIF::TunImpl {
  public:
    TunImpl(u_int32_t unit);
    int open_sock();
    void close_sock();
    int make_utun(const std::string& option);
  private:
    u_int32_t unit;
    int fd;
    std::string ifname();
    bool starts_with(const char* pre, const char* str);
    int ctl_id;
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

    this->ctl_id = ci.ctl_id;

    sockaddr_ctl sc{};

    sc.sc_id      = ci.ctl_id;
    sc.sc_len     = sizeof(sc);
    sc.sc_family  = AF_SYSTEM;
    sc.ss_sysaddr = AF_SYS_CONTROL;
    sc.sc_unit    = 0;

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

  int TunIF::TunImpl::make_utun(const std::string& option) {
    auto ifname = this->ifname();

    std::cout << "[INFO] Virtual Device: " << ifname << std::endl;

    std::string cmd = "ifconfig";

    return system((cmd + " " + ifname + " " + option).c_str());
  }

  TunIF::TunIF(u_int32_t unit): tun{std::make_unique<TunIF::TunImpl>(unit)} {}

  std::string TunIF::TunImpl::ifname() {
    struct ifaddrs *ifa;

    if (getifaddrs(&ifa) == -1) {
      return "";
    }

    std::vector<std::string> ifa_names;
    const std::string prefix = "utun";

    while (ifa != nullptr) {
      if (this->starts_with(prefix.c_str(), ifa->ifa_name)) {
        ifa_names.emplace_back(ifa->ifa_name);
      }
      ifa = ifa->ifa_next;
    }

    return ifa_names.at((unsigned long) this->ctl_id - 1);
  }

  // https://stackoverflow.com/questions/4770985/how-to-check-if-a-string-starts-with-another-string-in-c
  bool TunIF::TunImpl::starts_with(const char *pre, const char *str) {
    size_t len_pre = strlen(pre);
    size_t len_str = strlen(str);
    return len_str < len_pre ? false : strncmp(pre, str, len_pre) == 0;
  }

  void TunIF::start() {
    auto tun_ptr = this->tun.get();

    if (tun_ptr->open_sock() == 0) {
      std::cout << "[INFO] A tun interface has been created." << std::endl;
    } else {
      std::cerr << "[ERROR] Can't create a tun interface." << std::endl;
    }

    if (tun_ptr->make_utun("inet 10.0.7.1 10.0.7.1 mtu 1500 up") == 0) {

    }
    std::cout << "[INFO] TunIF has started." << std::endl;
  }

  void TunIF::stop() {
    auto tun_ptr = this->tun.get();
    tun_ptr->close_sock();
    std::cout << "[INFO] TunIF has stopped." << std::endl;
  }

}
