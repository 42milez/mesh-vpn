#pragma once

#include <string>

#include "boost/asio.hpp"

namespace network {
    struct IpNet {
        boost::asio::ip::address_v4 ip4;
        std::string netmask;
    };
}
