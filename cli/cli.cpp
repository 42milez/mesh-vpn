#include <csignal>
#include <iostream>
#include <thread>

#include "BuildInfo.h"
#include "mvcrypt/NetworkSecret.h"
#include "mvcore/Common.h"
#include "mvcore/AdminUtils.h"

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

        auto ns = mvcrypt::NetworkSecret{"10.7.0.0/24"};
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
      // join a network
      // ...
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
