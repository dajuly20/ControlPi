#!/bin/bash
source ~/.bashrc
rm -r bin
mkdir bin
cd bin
cmake ..
make && cd .. & h/createService.sh
