#!/bin/bash

workingdir=`pwd`
echo $workingdir

function ask_yes_or_no() {
    read -p "$1 ([y]es or [N]o): "
    case $(echo $REPLY | tr '[A-Z]' '[a-z]') in
        y|yes) echo "yes" ;;
        *)     echo "no" ;;
    esac
}

echo ""
echo ""
echo "Installation script for ControlPi"
echo ""
cd ~

#Test if SPI is enabled.
spiresponse=`lsmod | grep spi_`
if [ -z "$spiresponse" ]
then
echo "SPI is disabled! "
echo -e '\trun sudo raspi-config -> interfacing options -> spi -> enable'
echo -e "\tor remove comment before dtparam=spi=on  in /boot/config.txt"
echo -e "Then REBOOT and run this script again!" 
echo ""
echo ""
fail="true"
else
echo "SPi is enabled!"
echo "Found device $spiresponse"
fi

echo ""
echo "Swap should be at least 1000 MB for compiling!!"
if [[ "yes" == $(ask_yes_or_no "Edit swap?") ]]
then

sudo dphys-swapfile swapoff
sudo nano /etc/dphys-swapfile
sudo dphys-swapfile swapon
sudo service dphys-swapfile restart

fi

#Test if user is in group spi
if [ -z `groups | grep spi` ] && [ -z `groups | grep gpio`]
then
        echo "User $user is not in group spi. "
        if [[ "no" == $(ask_yes_or_no "Add user to group spi?") ]]
        then
        echo "Add user to group spi manually, then reboot or su $user"
        echo "Aborting."
        exit -1
        else
        echo "Adding user to group spi. "
        echo "You might have to re-execute this script"
        echo ""
        user=`whoami`
        sudo usermod -aG spi,gpio $user
	#if restart is needed for spi, group gets updated anyway.
	#if [ "$fail" != "true" ]
	#then
        exec sg spi newgrp `id -gn`
        #fi
	echo `groups`
        echo "User was added to group spi!" 
        echo "please execute su $user or reboot!" 
        echo ""
        echo ""
        fail="true"
        fi
        # Add user to group spi to allow spi communication
else    
        echo "User $user is already in groups spi and gpio."
fi



if [ "$fail" == "true" ]
then   
exit -1
fi


# though git might be installed when this script is within a git folder ;)  
if [ -z `which git` ] && [[ "yes" == $(ask_yes_or_no "Did not find git. Install?") ]] 
then
#Update system
sudo apt-get update
sudo apt-get -y upgrade
sudo apt-get install git -y
echo ""
echo "Upgrade complete.. "
fi


#Call sub-script to check for boost. 
pwd
cd $workingdir
./h/installBoost-1.6.9.sh
#Todo check if boost was installed by hand.
#package="libboost-all-dev"
#if [ $(dpkg-query -W -f='${Status}' $package 2>/dev/null | grep -c "ok installed") -eq 0 ]  &&  [[ "yes" == $(ask_yes_or_no "Did not find boostLibrary. Install $package? ") ]]
#then 
#sudo apt-get install $package -y
#fi

#if [ $(dpkg-query -W -f='${Status}' $package 2>/dev/null | grep -c "ok installed") -eq 0 ] 
#then
#echo "Boost Library is required! Aborting."
#exit -1
#fi

cd $workingdir
#Call sub script to check for BoostBeast.
./h/installBoostBeast.sh


# first change to home, clone libmcp23s17 and build.
mcp="libmcp23s17"
cd ~ 
if [ -d "$mcp" ]; then
	cd $mcp
	git pull

else
	git clone https://github.com/piface/$mcp
	cd $mcp
fi
sudo make
sudo make install

# first change to home, clone libpifacedigital and build.
mcp="libpifacedigitalcpp"
cd ~
if [ -d "$mcp" ]; then
        cd $mcp
        git pull

else
        git clone https://github.com/dajuly20/$mcp
        cd $mcp
fi
sudo make
sudo make install
sudo make example
cd   ~





cd $workingdir
#Trigger Script to update Cmake
./h/updateCmake.sh

#Trigger Script to update GCC
./h/install-gcc-8.2-ARM.sh
source ~/.bashrc








#clear
echo ""
echo "Installation complete!"
echo "Make sure the pifacedigital is properly attached" 

if [[ "yes" == $(ask_yes_or_no "Execute Test?") ]]
then
#executing test
cd $mcp
./example
cd ~
else
echo "Aborting."
exit -1
fi

if [[ "no" == $(ask_yes_or_no "Did the PiFace blink and click?") ]]
then
echo "Make sure the board is connected correctly and the  jumpers are set right (Adress 0) for the test"
echo "Also the LED's and the Relays can be enabled or disabled by jumpers."
echo "Aborting"
exit -1
fi


# Update This repo... just in case...
user="dajuly20"
mcp="ControlPi"
cd ~ 
if [ -d "$mcp" ]; then
        cd $mcp
        git pull

else
        git clone https://github.com/$user/$mcp
        cd $mcp
        git checkout WebSocketInclude #This can only be temporary ... replace on demand.
fi



echo ""
echo ""
echo "All dependencies set."
echo "Start Building Project... "
echo ""
echo ""
# Trigger Cmake and make
cd $workingdir
./h/rebuild.sh
echo ""
name="StartPiControl"
sudo rm ~/$name
ln  ./bin/ControlPi ~/$name
cd ~

if [[ "yes" == $(ask_yes_or_no "Start PiControl now?") ]]
then
if [[ "yes" == $(ask_yes_or_no "Start as Service?") ]]
	h/createService.sh
else
~/$name
fi
fi




