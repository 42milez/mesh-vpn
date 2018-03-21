#include <csignal>
#include <iostream>
#include <thread>
#include <numeric>
#include <cstdarg>

#include "libtorrent/alert_types.hpp"
#include "libtorrent/bdecode.hpp"
#include "libtorrent/ip_filter.hpp"
#include "libtorrent/session.hpp"

#include <boost/timer.hpp>

#include "BuildInfo.h"
#include "mvcrypt/NetworkSecret.h"
#include "mvcore/Common.h"

#include "mvcore/LocalNode.h"

namespace {

  // --------------------------------------------------
  //  Utils
  // --------------------------------------------------

  void help() {
    std::cout << "Usage mvpn [OPTIONS]" << std::endl
              << "Options: "            << std::endl << std::endl
              << "CLI usage:"           << std::endl;
  }

  void version() {
    std::cout << "Version: " << mvcore::Version << std::endl;
    std::cout << "Build: "   << DEV_QUOTED(BUILD_PLATFORM) << "/" << DEV_QUOTED(BUILD_TYPE) << std::endl;
  }

  // --------------------------------------------------
  //  Exit Handler
  // --------------------------------------------------

  class ExitHandler {
  public:
    static void toggle(int) { s_shouldExit = !s_shouldExit; }
    static bool shouldExit() { return s_shouldExit; }
  private:
    static bool s_shouldExit;
  };

  bool ExitHandler::s_shouldExit = false;

  // --------------------------------------------------
  //  Libtorrent
  // --------------------------------------------------

  //  namespace lt = libtorrent;
  //
  //  char const *timestamp() {
  //    time_t t = std::time(0);
  //    tm *timeinfo = std::localtime(&t);
  //    static char str[200];
  //    std::strftime(str, 200, "%b %d %X", timeinfo);
  //    return str;
  //  }
  //
  //  std::vector<lt::dht_lookup> dht_active_requests;
  //  std::vector<lt::dht_routing_bucket> dht_routing_table;
  //
  //  bool handle_alert(lt::alert *a) {
  //    // https://github.com/arvidn/libtorrent/issues/602
  //    if (auto p = lt::alert_cast<lt::dht_get_peers_reply_alert>(a)) {
  //      auto peers = p->peers();
  //      std::cout << "[ dht_get_peers_reply_alert ]" << std::endl;
  //      std::cout << " - info_hash : " << p->info_hash.to_string() << std::endl;
  //      std::cout << " - message   : " << p->message()             << std::endl;
  //      for (auto const& peer : peers) {
  //        std::cout << " - peer      : " << peer.address() << ":" << peer.port() << std::endl;
  //      }
  //      std::cout << std::endl;
  //      return true;
  //    }
  //
  //    return false;
  //  }

} // namespace

// --------------------------------------------------
//  main
// --------------------------------------------------

int main(int argc, char **argv) {

  //  Handle signals
  // --------------------------------------------------
  signal(SIGABRT, &ExitHandler::toggle);
  signal(SIGTERM, &ExitHandler::toggle);
  signal(SIGINT, &ExitHandler::toggle);

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
      mvnetwork::IpNet ipnet;
      std::tie(key, ipnet) = ns.secret();
      std::cout << key << std::endl;
      std::cout << ipnet.ip4.to_string() << std::endl;
      std::cout << ipnet.netmask << std::endl;
      auto marshaled = ns.marshal();
      std::cout << marshaled << std::endl;
      std::cout << "--------------------------------------------------" << std::endl;

      auto ns2 = mvcrypt::NetworkSecret::unmarshal(marshaled);
      std::string key2;
      mvnetwork::IpNet ipnet2;
      std::tie(key2, ipnet2) = ns2->secret();
      std::cout << "mvpn: " << key2 << std::endl;
      std::cout << "mvpn: " << ipnet2.ip4.to_string() << std::endl;
      std::cout << "mvpn: " << ipnet2.netmask << std::endl;
    }
  } else if (arg == "-j" || arg == "--join") {
    //    auto cache_size = 1024;
    //    auto active_loaded_limit = 20;
    //
    //    // --------------------------------------------------
    //
    //    lt::ip_filter loaded_ip_filter;
    //
    //    // --------------------------------------------------
    //
    //    lt::settings_pack settings;
    //    settings.set_int(lt::settings_pack::cache_size, cache_size);
    //    settings.set_int(lt::settings_pack::active_loaded_limit, active_loaded_limit);
    //    settings.set_int(lt::settings_pack::choking_algorithm, lt::settings_pack::rate_based_choker);
    //
    //    // --------------------------------------------------
    //
    //    auto listen_port = 6881;
    //    std::string bind_to_interface = "0.0.0.0";
    //
    //    // --------------------------------------------------
    //
    //    char iface_str[100];
    //    snprintf(iface_str, sizeof(iface_str), "%s:%d", bind_to_interface.c_str(), listen_port);
    //    settings.set_str(lt::settings_pack::listen_interfaces, iface_str);
    //
    //    // --------------------------------------------------
    //
    //    lt::session ses(settings);
    //
    //    settings.set_int(lt::settings_pack::alert_mask, lt::alert::all_categories);
    //    ses.set_ip_filter(loaded_ip_filter);
    //
    //    // --------------------------------------------------
    //
    //    lt::dht_settings dht;
    //    dht.privacy_lookups = true;
    //    ses.set_dht_settings(dht);
    //
    //    // --------------------------------------------------
    //
    //    lt::sha1_hash ih0 = lt::sha1_hash("3378363333347A743039326161646F7A31302E37");
    //
    //    // --------------------------------------------------
    //
    //    boost::timer t;
    //
    //    ses.dht_announce(ih0, 12345);
    //
    //    while (!ExitHandler::shouldExit()) {
    //
    //      if (t.elapsed() < 1) continue;
    //      t.restart();
    //
    //      ses.dht_get_peers(ih0);
    //
    //      // --------------------------------------------------
    //
    //      std::vector<lt::alert*> alerts;
    //      ses.pop_alerts(&alerts);
    //      std::string now = timestamp();
    //      for (auto alert : alerts) {
    //        try {
    //          if (!::handle_alert(alert)) {
    //            // do nothing
    //          }
    //        } catch (std::exception &e) {
    //          // do nothing
    //        }
    //      }
    //      alerts.clear();
    //    }

    auto lnode = new mvcore::LocalNode();

    lnode->start();

    while (!ExitHandler::shouldExit()) {
      // do something
      // ...
    }

    lnode->stop();
  } else {
    std::cerr << "Invalid argument: " << arg << std::endl;
    return -1;
  }

  return 0;
}
