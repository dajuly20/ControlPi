#!/bin/bash

function ask_yes_or_no() {
    read -p "$1 ([y]es or [N]o): "
    case $(echo $REPLY | tr '[A-Z]' '[a-z]') in
        y|yes) echo "yes" ;;
        *)     echo "no" ;;
    esac
}

function getBoostVers() {
#echo "Searching System for boost version"
# The {} supress compile errors..
{
VERS=$(echo -e '#include <boost/version.hpp>\nBOOST_VERSION' | gcc -s -x c++ -E - | grep "^[^#;]")
} &> /dev/null
echo $VERS
}


VERS="$(getBoostVers)"

VERS_SOLL="106900"

if [ -z "$VERS" ]; then
echo "Boost not installed."
elif [ "$VERS" = "$VERS_SOLL" ]; then

echo "Boost is already installed in the correct version! (1.6.9)"
exit 0
elif [ "$VERS" -gt "$VERS_SOLL" ]; then
	echo "The installed boost version is newer than the required $VERS... could work anyway.."
	echo "Installed: $VERS > $VERS_SOLL needed "
	if [[ "yes" == $(ask_yes_or_no "Skip installation then?") ]];then
		echo "execute $0 if you change your mind"
		exit 0
	fi
	INSTALLED="YES"
elif [ "$VERS_SOLL" -gt "$VERS" ]; then
	echo "The installed boost version is too old ($VERS)"
	INSTALLED="YES"
else
	echo "Could not determine boost version"
	echo ">>$VERS<<"
	echo ">>$VERS_SOLL<<"
	exit 0
fi


if [[ "$INSTALLED" == "YES" ]]; then
echo "You have another Version of boost installed."



#then 
#sudo apt-get install $package -y
#fi



if [[ "yes" == $(ask_yes_or_no "apt remove libboost-all-dev and libboost-dev?") ]];then   
package="libboost-all-dev"
if [ $(dpkg-query -W -f='${Status}' $package 2>/dev/null | grep -c "ok installed") -eq 0 ];then
sudo apt-get remove $package
fi

package="libboost-dev"
if [ $(dpkg-query -W -f='${Status}' $package 2>/dev/null | grep -c "ok installed") -eq 0 ]; then
sudo apt-get remove $package
fi
sudo apt-get autoremove

if [ -z $(getBoostVers) ]; then
echo "Uninstall successfull"
else
echo "Boost still installed. Try removing manually using locate boost/version.hpp"
locate boost/version.hpp
exit 1
fi
fi
else
echo "You shoud consider removing manually ( locate boost/version.hpp)"
locate boost/version.hpp

fi



APT_VERS=$(apt-cache policy libboost-all-dev | egrep -o "([0-9]{1,}\.)+[0-9]{1,}" | tail -n -1)
echo "Version in Apt-get is: $APT_VERS we need 1.6.9"
if [[ "yes" == $(ask_yes_or_no "Try Apt-get install?") ]];then
	sudo apt-get update
	sudo apt-get install libboost-all-dev=1.69.0
	VERS="$(getBoostVers)"
	if [ -z "$VERS" ]; then
		echo "Boost still not installed. Installing manually."
	elif [ "$VERS" = "$VERS_SOLL" ]; then
		echo "Boost was installed in version 1.69.0!"
		exit 0
	fi
fi


# Manual install..
cd ~
FILENAME=boost_1_69_0
wget -nc https://dl.bintray.com/boostorg/release/1.69.0/source/$FILENAME.tar.bz2

SHA_SOLL="8f32d4617390d1c2d16f26a27ab60d97807b35440d45891fa340fc2648b04406  $FILENAME.tar.bz2"
SHA_IS=`sha256sum $FILENAME.tar.bz2`

if [[ ! "$SHA_SOLL" == "$SHA_IS" ]]; then
echo "Checksum incorrect!"
echo "Should be: $SHA_SOLL"
echo "Is:        $SHA_IS"
exit 1
fi

#exit 1
echo "Extracting ... this could take some time ..."
tar xf $FILENAME.tar.bz2
cd $FILENAME
echo `pwd`
if [[ "yes" == $(ask_yes_or_no "Bootstrap & install? ~(20 min)") ]];then
	./bootstrap.sh
	./b2 -j  4
	sudo ./b2 install 
fi


# Final check for success
VERS="$(getBoostVers)"
if [ -z "$VERS" ]; then
         echo "Boost still not installed. Try re-executing h/$0"
elif [ "$VERS" = "$VERS_SOLL" ]; then
         echo "Boost was successfully installed in version 1.69.0!"
         exit 0
fi

