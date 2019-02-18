#!/bin/bash



function ask_yes_or_no() {
    read -p "$1 ([y]es or [N]o): "
    case $(echo $REPLY | tr '[A-Z]' '[a-z]') in
        y|yes) echo "yes" ;;
        *)     echo "no" ;;
    esac
}



dpkg -s libboost-dev | grep 'Version'
dpkg -s libboost-all-dev | grep 'Version'
sudo apt-get remove libboost-all-dev
sudo apt-get remove libboost-dev

cd ~
git clone https://github.com/boostorg/boost/
cd boost
echo `pwd`
if [[ "yes" == $(ask_yes_or_no "Bootstrap & install? ~(60 min)") ]];then
	./bootstrap.sh
	b2
fi
