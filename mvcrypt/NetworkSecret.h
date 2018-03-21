#pragma once

#include <cstddef>
#include <string>
#include <tuple>
#include "mvnetwork/IP.h"

#include "boost/asio.hpp"

namespace mvcrypt {

  class NetworkSecret {
  private:
    std::string key;
    mvnetwork::IpNet ipnet;

    NetworkSecret(std::string key, std::string cidr);

    std::string create_random_string(const std::string &candidate_chars, std::size_t length);

  public:
    NetworkSecret(std::string cidr);

    std::tuple<std::string, mvnetwork::IpNet> secret();

    std::string marshal();

    static NetworkSecret* unmarshal(std::string secret);
  };

} // namespace: mvcrypt
