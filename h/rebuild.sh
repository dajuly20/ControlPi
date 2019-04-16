#!/bin/bash
rm -r bin
mkdir bin
cd bin
cmake ..
cd ..
make && h/createService.sh
