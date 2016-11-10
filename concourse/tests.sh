#!/bin/bash

git clone https://github.com/sstephenson/bats
sudo bats/install.sh /usr/local
cd elara_github
sudo apt-get update -y
sudo apt-get install libblocksruntime-dev -y
TERM=xterm LD_LIBRARY_PATH=$(pwd)/Elara:$LD_LIBRARY_PATH make spec integration
