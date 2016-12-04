#!/bin/bash
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
