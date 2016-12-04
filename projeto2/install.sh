#!/bin/bash
echo "[*] Installing required libraries (curl, libgconf)"
DISTRO=$(cat /etc/*elease | grep -e ^ID= | cut -d = -f 2 | tr '[:upper:]' '[:lower:]')

if [[ "$DISTRO" -eq "ubuntu" ]] || [[ "$DISTRO" -eq "debian" ]]; then
  sudo apt-get install -y libgconf-2-4 curl
elif [[ "$DISTRO" -eq "arch" ]]; then
  sudo pacman -Su gconf curl
fi

echo "[*] Checking Node.js installation..."
VERSION=$(node -v)

if [[ -z "$VERSION" ]]; then
  echo "[-] Seems like Node.js isn't installed! Installing..."
  curl -o- https://raw.githubusercontent.com/creationix/nvm/v0.32.1/install.sh | bash
  export NVM_DIR="$HOME/.nvm"
  [ -s "$NVM_DIR/nvm.sh" ] && . "$NVM_DIR/nvm.sh"
  nvm install node
fi

echo "[*] Installing dependencies via NPM..."
npm install -g webpack electron
npm install
echo "[*] All done! use 'npm start' to start the application!"
