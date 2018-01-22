#include <csignal>
#include <iostream>
#include <thread>
#include <numeric>
#include <cstdarg>

#include "libtorrent/config.hpp"
#include "libtorrent/session.hpp"
#include "libtorrent/kademlia/node.hpp" // for verify_message
#include "libtorrent/bencode.hpp"
#include "libtorrent/socket_io.hpp" // for hash_address
#include "libtorrent/broadcast_socket.hpp" // for supports_ipv6
#include "libtorrent/performance_counters.hpp" // for counters
#include "libtorrent/random.hpp"
#include "libtorrent/ed25519.hpp"

#include "libtorrent/kademlia/node_id.hpp"
#include "libtorrent/kademlia/routing_table.hpp"
#include "libtorrent/kademlia/item.hpp"
#include "libtorrent/kademlia/dht_observer.hpp"
#include "libtorrent/ed25519.hpp"

#include "BuildInfo.h"
#include "mvcrypt/NetworkSecret.h"
#include "mvcore/Common.h"
#include "mvcore/AdminUtils.h"

namespace lt = libtorrent;

void help() {
  std::cout << "Usage vpn [OPTIONS]" << std::endl
            << "Options: " << std::endl << std::endl
            << "CLI usage:" << std::endl;
  exit(0);
}

void version() {
  std::cout << "Version: " << dev::Version << std::endl;
  std::cout << "Build: " << DEV_QUOTED(BUILD_PLATFORM) << "/" << DEV_QUOTED(BUILD_TYPE) << std::endl;
  exit(0);
}

class ExitHandler : public dev::rpc::SystemManager {
public:
  void exit() { exitHandler(0); }

  static void exitHandler(int) { s_shouldExit = true; }

  bool shouldExit() const { return s_shouldExit; }

private:
  static bool s_shouldExit;
};

// static変数の初期化？
bool ExitHandler::s_shouldExit = false;

// --------------------------------------------------

std::list<std::pair<lt::udp::endpoint, lt::entry> > g_sent_packets;

struct mock_socket : lt::dht::udp_socket_interface {
  bool has_quota() TORRENT_OVERRIDE { return true; }

  bool send_packet(lt::entry &msg, lt::udp::endpoint const &ep, int flags) TORRENT_OVERRIDE {
    // TODO: ideally the mock_socket would contain this queue of packets, to
    // make tests independent
    g_sent_packets.emplace_back(std::make_pair(ep, msg));
    return true;
  }
};

struct obs : lt::dht::dht_observer {
  void set_external_address(lt::address const &addr, lt::address const &source) TORRENT_OVERRIDE {}

  lt::address external_address() TORRENT_OVERRIDE {
    return lt::address_v4::from_string("236.0.0.1");
  }

  void get_peers(lt::sha1_hash const &ih) TORRENT_OVERRIDE {}

  void outgoing_get_peers(lt::sha1_hash const &target, lt::sha1_hash const &sent_target,
                          lt::udp::endpoint const &ep) TORRENT_OVERRIDE {}

  void announce(lt::sha1_hash const &ih, lt::address const &addr, int port) TORRENT_OVERRIDE {}

  void log(dht_logger::module_t l, char const *fmt, ...) TORRENT_OVERRIDE {
    va_list v;
    va_start(v, fmt);
    char buf[1024];
    vsnprintf(buf, sizeof(buf), fmt, v);
    va_end(v);
    m_log.emplace_back(buf);
  }

  void log_packet(message_direction_t dir, char const *pkt, int len, lt::udp::endpoint node) TORRENT_OVERRIDE {}

  bool on_dht_request(char const *query, int query_len, lt::dht::msg const &request,
                      lt::entry &response) TORRENT_OVERRIDE {
    return false;
  }

  std::vector<std::string> m_log;
};

lt::sha1_hash generate_next() {
  lt::sha1_hash ret;
  for (int i = 0; i < 20; ++i) ret[i] = rand() & 0xff;
  return ret;
}

std::list<std::pair<lt::udp::endpoint, lt::entry> >::iterator
find_packet(lt::udp::endpoint ep) {
  return std::find_if(g_sent_packets.begin(), g_sent_packets.end(), boost::bind(&std::pair<lt::udp::endpoint,
    lt::entry>::first, _1) == ep);
}

void write_peers(lt::entry::dictionary_type &r, std::set<lt::tcp::endpoint> const &peers) {
  lt::entry::list_type &pe = r["values"].list();
  for (auto it = peers.begin(); it != peers.end(); ++it) {
    std::string endpoint(18, '\0');
    std::string::iterator out = endpoint.begin();
    libtorrent::detail::write_endpoint(*it, out);
    endpoint.resize(out - endpoint.begin());
    pe.emplace_back(lt::entry(endpoint));
  }
}

struct msg_args {
  msg_args &info_hash(char const *i) {
    if (i) a["info_hash"] = std::string(i, 20);
    return *this;
  }

  msg_args &name(char const *n) {
    if (n) a["n"] = n;
    return *this;
  }

  msg_args &token(std::string t) {
    a["token"] = t;
    return *this;
  }

  msg_args &port(int p) {
    a["port"] = p;
    return *this;
  }

  msg_args &target(char const *t) {
    if (t) a["target"] = std::string(t, 20);
    return *this;
  }

  msg_args &value(lt::entry const &v) {
    a["v"] = v;
    return *this;
  }

  msg_args &scrape(bool s) {
    a["scrape"] = s ? 1 : 0;
    return *this;
  }

  msg_args &seed(bool s) {
    a["seed"] = s ? 1 : 0;
    return *this;
  }

  msg_args &key(std::string k) {
    a["k"] = k;
    return *this;
  }

  msg_args &sig(std::string s) {
    a["sig"] = s;
    return *this;
  }

  msg_args &seq(int s) {
    a["seq"] = s;
    return *this;
  }

  msg_args &cas(boost::int64_t c) {
    a["cas"] = c;
    return *this;
  }

  msg_args &nid(lt::sha1_hash const &n) {
    a["id"] = n.to_string();
    return *this;
  }

  msg_args &salt(char const *s) {
    if (s) a["salt"] = s;
    return *this;
  }

  msg_args &want(std::string w) {
    a["want"].list().push_back(w);
    return *this;
  }

  msg_args &nodes(lt::dht::nodes_t const &n) {
    if (!n.empty()) lt::dht::write_nodes_entry(a, n);
    return *this;
  }

  msg_args &peers(std::set<lt::tcp::endpoint> const &p) {
    if (!p.empty()) write_peers(a.dict(), p);
    return *this;
  }

  lt::entry a;
};

void lazy_from_entry(lt::entry const &e, lt::bdecode_node &l) {
  lt::error_code ec;
  static char inbuf[1500];
  int len = lt::bencode(inbuf, e);
  int ret = bdecode(inbuf, inbuf + len, l, ec);
}

void send_dht_request(lt::dht::node &node, char const *msg, lt::udp::endpoint const &ep, lt::bdecode_node *reply,
                      msg_args const &args = msg_args(), char const *t = "10", bool has_response = true) {
  // we're about to clear out the backing buffer
  // for this lazy_entry, so we better clear it now
  reply->clear();
  lt::entry e;
  e["q"] = msg;
  e["t"] = t;
  e["y"] = "q";
  e["a"] = args.a;
  e["a"].dict().insert(std::make_pair("id", generate_next().to_string()));
  char msg_buf[1500];
  int size = bencode(msg_buf, e);

  lt::bdecode_node decoded;
  lt::error_code ec;
  lt::bdecode(msg_buf, msg_buf + size, decoded, ec);
  if (ec) fprintf(stdout, "bdecode failed: %s\n", ec.message().c_str());

  lt::dht::msg m(decoded, ep);
  node.incoming(m);

  // If the request is supposed to get a response, by now the node should have
  // invoked the send function and put the response in g_sent_packets
  std::list<std::pair<lt::udp::endpoint, lt::entry> >::iterator i = find_packet(ep);
  if (has_response) {
    if (i == g_sent_packets.end()) {
      return;
    }

    lazy_from_entry(i->second, *reply);
    g_sent_packets.erase(i);

    return;
  }

  // this request suppose won't be responsed.
  if (i != g_sent_packets.end()) {
    return;
  }
}

// --------------------------------------------------

int main(int argc, char **argv) {

  //  Parse arguments
  // --------------------------------------------------
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (false) {
      // do nothing
    } else if (arg == "-h" || arg == "--help") {
      help();
    } else if (arg == "-v" || arg == "--version") {
      version();
    } else if (arg == "-n" || arg == "--new") {
      if (false) {
        // キーが引数に与えられている場合（既存のキーからシークレットを復元する操作）
        //  - キーは16進数となるため、16進数とbyte型の相互変換を行う仕組みが必要
        //  - 複数引数を扱う必要がある
        // ...
      } else {
        std::cout << "sizeof(char): " << sizeof(char) << std::endl;
        std::cout << "--------------------------------------------------" << std::endl;

        auto ns = mvcrypt::NetworkSecret{ "10.7.0.0/24" };
        std::string key;
        network::IpNet ipnet;
        std::tie(key, ipnet) = ns.secret();
        std::cout << key << std::endl;
        std::cout << ipnet.ip4.to_string() << std::endl;
        std::cout << ipnet.netmask << std::endl;
        auto marshaled = ns.marshal();
        std::cout << marshaled << std::endl;
        std::cout << "--------------------------------------------------" << std::endl;

        auto ns2 = mvcrypt::NetworkSecret::unmarshal(marshaled);
        std::string key2;
        network::IpNet ipnet2;
        std::tie(key2, ipnet2) = ns2->secret();
        std::cout << "cli: " << key2 << std::endl;
        std::cout << "cli: " << ipnet2.ip4.to_string() << std::endl;
        std::cout << "cli: " << ipnet2.netmask << std::endl;
      }
    } else if (arg == "-p" || arg == "--ping") {
      // Perform a DHT ping to another node
      // ...
    } else if (arg == "-j" || arg == "--join") {
      lt::dht_settings settings;
      settings.max_torrents = 4;
      settings.max_dht_items = 4;
      settings.enforce_node_id = false;
      mock_socket s;
      obs observer;
      lt::counters cnt;
      lt::dht::node node(&s, settings, lt::dht::node_id(0), &observer, cnt);

      lt::bdecode_node response;
      char error_string[200];
      bool ret;

      // ===== [ ping ] =====

      lt::udp::endpoint source(lt::address::from_string("10.0.0.1"), 20);
      send_dht_request(node, "ping", source, &response);

      lt::dht::key_desc_t pong_desc[] = {
        { "y",  lt::bdecode_node::string_t, 1,  0 },
        { "t",  lt::bdecode_node::string_t, 2,  0 },
        { "r",  lt::bdecode_node::dict_t,   0,  lt::dht::key_desc_t::parse_children },
        { "id", lt::bdecode_node::string_t, 20, lt::dht::key_desc_t::last_child },
      };

      lt::bdecode_node pong_keys[4];

      fprintf(stdout, "msg: %s\n", print_entry(response).c_str());
      ret = lt::dht::verify_message(response, pong_desc, pong_keys, error_string, sizeof(error_string));
      if (ret) {
        std::cout << "valid ping response" << std::endl;
      } else {
        fprintf(stdout, "invalid ping response: %s\n", error_string);
      }
    } else if (arg == "-l" || arg == "--list") {
      // list peers
      // ...
    } else {
      std::cerr << "Invalid argument: " << arg << std::endl;
      exit(-1);
    }
  }

  //  Handle signals
  // --------------------------------------------------
  ExitHandler exitHandler;

  signal(SIGABRT, &ExitHandler::exitHandler);
  signal(SIGTERM, &ExitHandler::exitHandler);
  signal(SIGINT, &ExitHandler::exitHandler);

//  while (!exitHandler.shouldExit()) {
//
//  }

  return 0;
}
