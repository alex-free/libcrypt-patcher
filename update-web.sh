#!/bin/bash

web_page="/home/alex/dev/alex-free.github.io/libcrypt-patcher"
set -e

cd "$(dirname "$0")"
cp -rv *.md "$web_page"
rm -rf "$web_page"/images
cp -rv images "$web_page"
mv "$web_page"/readme.md "$web_page"/index.md
