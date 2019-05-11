#!/bin/sh
cd ~
FOLDER=beast
if [ ! -d "$FOLDER" ] ; then
git clone https://github.com/boostorg/$FOLDER
fi

cd beast
git pull
sudo cp -R include/. /usr/include 
