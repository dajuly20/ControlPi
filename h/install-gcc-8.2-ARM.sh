#!/bin/bash
# Fow ARM (RapberrPi) only
cd ~
reponame=raspberry-pi-gcc-binary
git clone "https://bitbucket.org/sol_prog/$reponame"
cd $reponame
tar xf gcc-8.1.0.tar.bz2
sudo mv gcc-8.1.0 /usr/local
export PATH=/usr/local/gcc-8.1.0/bin:$PATH
echo 'export PATH=/usr/local/gcc-8.1.0/bin:$PATH' >> ~/.bashrc
source ~/.bashrc
export CC=/usr/local/gcc-8.1.0/bin/gcc-8.1.0
export CXX=/usr/local/gcc-8.1.0/bin/g++-8.1.0
#export CC=/usr/local/bin/gcc  #Original values
#export CXX=/usr/local/bin/g++

cd ~ 

if [[ ! -z `which gcc-8.1.0` ]]
then
echo "GCC 8.1.0 installed  sucessfully"
/bin/rm -rf "~/$reponame"
else
echo "Error during install!"
fi

