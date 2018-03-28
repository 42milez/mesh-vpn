#!/usr/bin/env sh

USERNAME=$(id -u -n)

if [ -e ./mvpn ]; then
  sudo chown ${USERNAME} mvpn
fi

"/Users/${USERNAME}/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/173.4674.29/CLion.app/Contents/bin/cmake/bin/cmake" --build /Users/k12n3ud0n/Workspace/mesh-vpn/cmake-build-debug --target mvpn -- -j 4

sudo chown root mvpn
sudo chmod ugo+s mvpn
