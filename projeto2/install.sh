#!/bin/bash
echo "[*] Checking Node.js installation..."
VERSION=$(node -v)
ERROR=false

if [[ -z "$VERSION" ]]; then
  echo "[-] Seems like Node.js isn't installed! Installing..."
  DISTRO=$(cat /etc/*elease | grep -e ^ID= | cut -d = -f 2 | tr '[:upper:]' '[:lower:]')
  if [[ "$DISTRO" -eq "ubuntu" ]] || [[ "$DISTRO" -eq "debian" ]]; then
    curl -sL https://deb.nodesource.com/setup_7.x | sudo -E bash -
    apt-get install -y nodejs
  elif [ "$DISTRO" -eq "arch" ]; then
    pacman -S nodejs npm
  else
    ERROR=true
    echo "[-] This script only supports Arch Linux and Debian based distros! Try to install nodejs and npm manually and run 'npm install' to install project dependencies"
  fi
fi

if [[ "$ERROR" -eq false ]]; then
  echo "[*] Installing dependencies via NPM..."
  npm install
  npm install -g webpack electron
  echo "[*] All done! use 'npm start' to start the application!"
fi
