#!/bin/bash
# Fow ARM (RapberrPi) only
cd ~
reponame=raspberry-pi-gcc-binary
git clone "https://bitbucket.org/sol_prog/$reponame"
cd $reponame
tar xf gcc-8.1.0.tar.bz2
sudo rm -r /usr/local/gcc-8.1.0/
sudo mv gcc-8.1.0 /usr/local
echo 'export CC=/usr/local/gcc-8.1.0/bin/gcc-8.1.0' >> ~/.bashrc
echo 'export CXX=/usr/local/gcc-8.1.0/bin/g++-8.1.0' >> ~/.bashrc
export PATH=/usr/local/gcc-8.1.0/bin:$PATH
echo 'export PATH=/usr/local/gcc-8.1.0/bin:$PATH' >> ~/.bashrc
source ~/.bashrc
#export CC=/usr/local/bin/gcc  #Original values
#export CXX=/usr/local/bin/g++

cd ~ 

if [[ ! -z `which gcc-8.1.0` ]]
then
echo "GCC 8.1.0 installed  sucessfully"
/bin/rm -rf "~/$reponame"

if [[ ! -z $CXX ]]
then
echo "CXX Variable found!"
else
echo "CXX Variable NOT found. TRY source ~/.bashrc or reboot"
fi
else
echo "Error during install!"
fi

