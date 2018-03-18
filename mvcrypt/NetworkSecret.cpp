#include <iostream>
#include <random>

#include "boost/algorithm/string.hpp"
#include "NetworkSecret.h"
#include "mvutil/Util.h"

namespace mvcrypt {

  const std::string key_candidates = "abcdefghijklmnopqrstuvwxyz0123456789";
  const int key_length = 16;

  NetworkSecret::NetworkSecret(std::string cidr) {
    this->key = create_random_string(key_candidates, key_length);

    std::vector<std::string> net_info;
    boost::split(net_info, cidr, boost::is_any_of("/"));

    if (net_info.size() < 2) {
      exit(1);
    }

    this->ipnet = mvnetwork::IpNet{
      boost::asio::ip::address_v4::from_string(net_info[0]),
      net_info[1]
    };
  }

  NetworkSecret::NetworkSecret(std::string key, std::string cidr) {
    this->key = key;

    std::vector<std::string> net_info;
    boost::split(net_info, cidr, boost::is_any_of("/"));

    if (net_info.size() < 2) {
      exit(1);
    }

    this->ipnet = mvnetwork::IpNet{
      boost::asio::ip::address_v4::from_string(net_info[0]),
      net_info[1]
    };
  }

  std::string NetworkSecret::create_random_string(const std::string &candidates, std::size_t length) {
    std::random_device engine;
    std::uniform_int_distribution<std::size_t> dist(0, candidates.size() - 1);
    std::string str;

    for (std::size_t i = 0; i < length; ++i) {
      std::size_t idx = dist(engine);
      str += candidates[idx];
    }

    return str;
  }

  std::tuple<std::string, mvnetwork::IpNet> NetworkSecret::secret() {
    return std::make_tuple(this->key, this->ipnet);
  };

  std::string NetworkSecret::marshal() {
    std::string s = this->key + this->ipnet.ip4.to_string() + this->ipnet.netmask;
    std::cout << s << std::endl;
    return mvutil::string_to_hex(s);
  }

  NetworkSecret* NetworkSecret::unmarshal(const std::string secret) {
    std::string s = mvutil::hex_to_string(secret);
    std::cout << s << std::endl;

    auto ip_address_len = s.length() - 16 - 2;

    std::string key = s.substr(0, 16);
    std::cout << key << std::endl;

    std::string ip_address = s.substr(16, ip_address_len);
    std::cout << ip_address << std::endl;

    auto netmask = s.substr(16 + ip_address_len, 2);
    std::cout << netmask << std::endl;

    return new NetworkSecret{ key, ip_address + "/" + netmask };
  }

}
