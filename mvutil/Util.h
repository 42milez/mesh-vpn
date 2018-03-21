//
// Created by Akihiro TAKASE on 2017/12/03.
//

#pragma once

#ifndef MESH_VPN_UTIL_H
#define MESH_VPN_UTIL_H

#include <string>

namespace mvutil {

  std::string string_to_hex(const std::string &input);
  std::string hex_to_string(const std::string &input);

} // namespace: mvutil

#endif //MESH_VPN_UTIL_H
