#!/bin/bash
# TO Upgrade cmake to 3.10
which cmake
INSTALLED_VERS=`cmake --version | head -n 1`
# Download version
CMAKE_VERS="3.13.4"

function ask_yes_or_no() {
    read -p "$1 ([y]es or [N]o): "
    case $(echo $REPLY | tr '[A-Z]' '[a-z]') in
        y|yes) echo "yes" ;;
        *)     echo "no" ;;
    esac
}


#        echo -n "Password for $USER:"
#        read -s  PASS
        echo
        if [[ "yes" == $(ask_yes_or_no "Uninstall $INSTALLED_VERS and replace by $CMAKE_VERS?") ]]
         then
	 sudo apt-get remove cmake
	 sudo apt-get autoremove
	fi


        if [[ "yes" == $(ask_yes_or_no "Download version $CMAKE_VERS nach home?") ]]
	then
	 cd ~
	 wget "https://github.com/Kitware/CMake/releases/download/v$CMAKE_VERS/cmake-$CMAKE_VERS.tar.gz"
	        if [[ "yes" == $(ask_yes_or_no "Dauer ca. 20 Min auf Pi \nDownload erfolgreich. Entpacken, bauen und Installieren?") ]]
		then
		 extract cmake-$CMAKE_VERS.tar.gz 
	         cd cmake-$CMAKE_VERS
		 ./bootstrap
		 make -j 6
		 sudo make install
		fi
	fi
	
