#!/bin/bash

echo "[*] Checking for electron-packager..."
PACK=$(npm list -g electron-packager)

if [[ -z "$PACK" ]]; then
  echo "[*] Installing electron-packager via npm"
  npm install electron-packager -g
fi

echo "[*] Cleaning dist directory..."
rm -rf dist/*

echo "[*] Generating linux x86 and x86-64 executables..."
electron-packager ./ NativeIDE --out=dist --platform=linux --arch=x64
electron-packager ./ NativeIDE --out=dist --platform=linux --arch=ia32

echo "[*] All executables generated under 'dist' directory!"
