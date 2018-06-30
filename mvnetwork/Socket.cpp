#include <iostream>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <sys/event.h>

#include "Socket.h"

namespace mvnetwork {

  Socket::Socket(const int fd) {
    fd_ = fd;
    create_multiplexer();
  }

  Socket::Socket(std::unique_ptr<NetworkInterface> ni) {
    create_socket(ni->port);
    create_multiplexer();
  }

  Socket::~Socket() {
    close(fd_);
  }

  void Socket::create_socket(const int port) {

    //  Create socket
    // --------------------------------------------------

    struct addrinfo hints { AI_PASSIVE, AF_INET, SOCK_STREAM, 0, 0, nullptr, nullptr, nullptr };
    struct addrinfo *res0;

    int err;

    if ((err = getaddrinfo(nullptr, std::to_string(port).data(), &hints, &res0)) != 0) {
      std::cout << "getaddrinfo(): " << gai_strerror(err) << std::endl;
      return;
    }

    char nbuf[NI_MAXHOST];
    char sbuf[NI_MAXSERV];

    if ((err = getnameinfo(res0->ai_addr, res0->ai_addrlen,
                           nbuf, sizeof(nbuf),
                           sbuf, sizeof(sbuf),
                           NI_NUMERICHOST | NI_NUMERICSERV)) != 0) {
      std::cout << "getnameinfo(): " << gai_strerror(err) << std::endl;
      freeaddrinfo(res0);
      return;
    }

    if ((fd_ = socket(res0->ai_family, res0->ai_socktype, res0->ai_protocol)) == -1) {
      std::cout << "[ERROR] socket()" << std::endl;
      freeaddrinfo(res0);
      return;
    }

    int opt { 1 };
    socklen_t opt_len { sizeof(opt) };

    if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, opt_len) == -1) {
      std::cout << "[ERROR] setsockopt()" << std::endl;
      close(fd_);
      freeaddrinfo(res0);
      return;
    }

    if (bind(fd_, res0->ai_addr, res0->ai_addrlen) == -1) {
      std::cout << "[ERROR] bind()" << std::endl;
      close(fd_);
      freeaddrinfo(res0);
      return;
    }

    if (listen(fd_, SOMAXCONN) == -1) {
      std::cout << "[ERROR] listen()" << std::endl;
      close(fd_);
      freeaddrinfo(res0);
      return;
    }

    freeaddrinfo(res0);
  }

  void Socket::create_multiplexer() {
    mux_ = kqueue();
    if (mux_ == -1) {
      std::cout << "[ERROR] kqueue" << std::endl;
      return;
    }
    struct kevent event{ (uintptr_t) fd_, EVFILT_READ, EV_ADD|EV_CLEAR, 0, 0, nullptr };
    kevent(mux_, &event, 1, nullptr, 0, nullptr);
  }

  void Socket::wait(std::function<void(const int fd)> fn) {
    struct kevent events[10];
    auto nfds = kevent(mux_, nullptr, 0, events, 10, nullptr);
    if (nfds == -1) {
      std::cout << "[ERROR] kevent" << std::endl;
      return;
    } else if (nfds == 0) {
      std::cout << "[ERROR] timeout" << std::endl;
      return;
    } else {
      for (auto i = 0; i < nfds; i++) {
        auto soc = (int) events[i].ident;
        if (soc == fd_) {
          struct sockaddr_storage addr{};
          socklen_t socklen = sizeof(addr);
          auto fd = accept(soc, (struct sockaddr *) &addr, &socklen);
          fn(fd);
        }
      }
    }
  }

}
