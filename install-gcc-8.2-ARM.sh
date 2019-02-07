#!/bin/bash
# Fow ARM (RapberrPi) only
cd ~
git clone https://bitbucket.org/sol_prog/raspberry-pi-gcc-binary 
cd rasperry-pi-gcc-binary
tar xf gcc-8.1.0.tar.bz2
sudo mv gcc-8.1.0 /usr/local
export PATH=/usr/local/gcc-8.1.0/bin:$PATH
echo 'export PATH=/usr/local/gcc-8.1.0/bin:$PATH' >> ~/.bashrc
source .bashrc
export CC=/usr/local/gcc-8.1.0/bin/gcc-8.1.0
export CXX=/usr/local/gcc-8.1.0/bin/g++-8.1.0
#export CC=/usr/local/bin/gcc  #Original values
#export CXX=/usr/local/bin/g++

cd ~ 
echo "When you see the path of newly installed gcc-8.1.0 below it should have worked"
which gcc-8.1.0
echo "When installed sucessfully, dont forget to remove directory rasperry-pi-gcc-binary"
