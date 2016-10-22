#!/bin/bash
echo "[*] Checking Node.js installation..."
VERSION=$(node -v)

if [[ -z "$VERSION" ]]; then
  echo "[-] Seems like Node.js isn't installed! Installing..."
  apt-get install -y nodejs npm
fi

echo "[*] Installing dependencies via NPM..."
npm install
echo "[*] All done! use 'npm start' to start the application!"
