#!/usr/bin/env bash

echo "Note: this will only work on machines where we can install to the user's bin or lib path"
echo "Else just download llv.h"

curl -s https://api.github.com/repos/BraedonWooding/LLV/releases/latest \
    | grep "browser_download_url" \
    | cut -d '"' -f 4 \
    | wget -qi -

mv libLLV.a /usr/local/lib/
mkdir /usr/local/include/LLV
mv include.zip /usr/local/include/LLV/
cd /usr/local/include/LLV/
unzip -qq include.zip
mv include/* ./
rmdir include
rm include.zip
