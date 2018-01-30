#include <csignal>
#include <iostream>
#include <thread>
#include <numeric>
#include <cstdarg>

#include "libtorrent/alert_types.hpp"
#include "libtorrent/bdecode.hpp"
#include "libtorrent/ip_filter.hpp"
#include "libtorrent/session.hpp"
#include "print.hpp"
#include "session_view.hpp"
#include "torrent_view.hpp"

#include "BuildInfo.h"
#include "mvcrypt/NetworkSecret.h"
#include "mvcore/Common.h"
#include "mvcore/AdminUtils.h"

namespace lt = libtorrent;

void help() {
  std::cout << "Usage vpn [OPTIONS]" << std::endl
            << "Options: "           << std::endl << std::endl
            << "CLI usage:"          << std::endl;
  exit(0);
}

void version() {
  std::cout << "Version: " << dev::Version << std::endl;
  std::cout << "Build: "   << DEV_QUOTED(BUILD_PLATFORM) << "/" << DEV_QUOTED(BUILD_TYPE) << std::endl;
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

bool ExitHandler::s_shouldExit = false;

char const *timestamp() {
  time_t t = std::time(0);
  tm *timeinfo = std::localtime(&t);
  static char str[200];
  std::strftime(str, 200, "%b %d %X", timeinfo);
  return str;
}

std::vector<lt::dht_lookup> dht_active_requests;
std::vector<lt::dht_routing_bucket> dht_routing_table;

bool handle_alert(torrent_view &view, session_view &ses_view, lt::session &ses, lt::alert *a) {
  if (lt::session_stats_alert * s = lt::alert_cast<lt::session_stats_alert>(a)) {
    ses_view.update_counters(s->values,
                             sizeof(s->values) / sizeof(s->values[0]),
                             lt::duration_cast<lt::microseconds>(s->timestamp().time_since_epoch()).count());
    return true;
  }

  if (lt::dht_stats_alert * p = lt::alert_cast<lt::dht_stats_alert>(a)) {
    dht_active_requests = p->active_requests;
    dht_routing_table = p->routing_table;
    return true;
  }

  return false;
}

int main(int argc, char **argv) {

  //  Handle signals
  // --------------------------------------------------
  ExitHandler exitHandler;

  signal(SIGABRT, &ExitHandler::exitHandler);
  signal(SIGTERM, &ExitHandler::exitHandler);
  signal(SIGINT, &ExitHandler::exitHandler);

  //  Parse arguments
  // --------------------------------------------------
  std::string arg = argv[1];

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
  } else if (arg == "-j" || arg == "--join") {
    auto cache_size = 1024;
    auto active_loaded_limit = 20;

    // --------------------------------------------------

    lt::ip_filter loaded_ip_filter;

    // --------------------------------------------------

    lt::settings_pack settings;
    settings.set_int(lt::settings_pack::cache_size, cache_size);
    settings.set_int(lt::settings_pack::active_loaded_limit, active_loaded_limit);
    settings.set_int(lt::settings_pack::choking_algorithm, lt::settings_pack::rate_based_choker);

    // --------------------------------------------------

    auto listen_port = 6881;
    std::string bind_to_interface = "0.0.0.0";

    // --------------------------------------------------

    char iface_str[100];
    snprintf(iface_str, sizeof(iface_str), "%s:%d", bind_to_interface.c_str(), listen_port);
    settings.set_str(lt::settings_pack::listen_interfaces, iface_str);

    // --------------------------------------------------

    lt::session ses(settings);

    // --------------------------------------------------

    ses.set_ip_filter(loaded_ip_filter);

    // --------------------------------------------------

    lt::error_code ec;

    // --------------------------------------------------

    lt::dht_settings dht;
    dht.privacy_lookups = true;
    ses.set_dht_settings(dht);

    // --------------------------------------------------

    lt::sha1_hash ih = lt::hasher("test", 5).final();

    torrent_view view;
    session_view ses_view;

    std::deque <std::string> events;

    std::string out;
    char str[500];
    int pos = view.height() + ses_view.height();
    set_cursor_pos(0, pos);

    int cache_flags = lt::session::disk_cache_no_pieces;
    lt::torrent_handle h = view.get_active_handle();

    lt::cache_status cs;
    ses.get_cache_info(&cs, h, cache_flags);

    // --------------------------------------------------

    while (!exitHandler.shouldExit()) {

      ses.dht_get_peers(ih);

      // --------------------------------------------------

      std::vector<lt::alert*> alerts;
      ses.pop_alerts(&alerts);
      std::string now = timestamp();
      for (auto i = alerts.begin(), end(alerts.end()); i != end; ++i) {
        try {
          if (!::handle_alert(view, ses_view, ses, *i)) {
            std::string event_string;
            events.emplace_back(event_string);
            if (events.size() >= 20) events.pop_front();
          }
        } catch (std::exception &e) {
          // do nothing
        }
      }
      alerts.clear();

      // --------------------------------------------------

      int bucket = 0;
      for (auto i = dht_routing_table.begin(), end(dht_routing_table.end()); i != end; ++i, ++bucket) {
        char const *progress_bar = "################################"
                                   "################################"
                                   "################################"
                                   "################################";
        char const *short_progress_bar = "--------";
        snprintf(str,
                 sizeof(str),
                 "%3d [%3d, %d] %s%s\x1b[K\n",
                 bucket,
                 i->num_nodes,
                 i->num_replacements,
                 progress_bar + (128 - i->num_nodes),
                 short_progress_bar + (8 - (std::min)(8, i->num_replacements)));
        out += str;
        pos += 1;
      }

      for (auto i = dht_active_requests.begin(), end(dht_active_requests.end()); i != end; ++i) {
        snprintf(str,
                 sizeof(str), "  %10s [limit: %2d] "
                 "in-flight: %-2d "
                 "left: %-3d "
                 "1st-timeout: %-2d "
                 "timeouts: %-2d "
                 "responses: %-2d "
                 "last_sent: %-2d\x1b[K\n",
                 i->type,
                 i->branch_factor,
                 i->outstanding_requests,
                 i->nodes_left,
                 i->first_timeout,
                 i->timeouts,
                 i->responses,
                 i->last_sent);
        out += str;
        pos += 1;
      }

      // --------------------------------------------------

      out += "\x1b[J";
      print(out.c_str());
      fflush(stdout);
    }
  } else {
    std::cerr << "Invalid argument: " << arg << std::endl;
    exit(-1);
  }

  return 0;
}
