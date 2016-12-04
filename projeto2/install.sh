#!/bin/bash
echo "[*] Checking Node.js installation..."
VERSION=$(node -v)

if [[ -z "$VERSION" ]]; then
  echo "[-] Seems like Node.js isn't installed! Installing..."
  curl -o- https://raw.githubusercontent.com/creationix/nvm/v0.32.1/install.sh | bash
  nvm install node
fi

echo "[*] Installing dependencies via NPM..."
npm install
npm install -g webpack electron
echo "[*] All done! use 'npm start' to start the application!"
