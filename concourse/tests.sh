#!/bin/bash

cd elara
sudo apt-get update -y
sudo apt-get install libblocksruntime-dev -y
LD_LIBRARY_PATH=$(pwd)/Elara:$LD_LIBRARY_PATH make spec
