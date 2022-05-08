#!/bin/bash
#Add contrpipi user if not exits and link binary to  /usr/bin
sudo id -u controlpi &>/dev/null || sudo useradd controlpi
sudo usermod -a -G spi controlpi
sudo usermod -a -G gpio controlpi
sudo ln bin/ControlPi /usr/bin/ControlPi
sudo ln -s `realpath conf` /opt/controlpi
