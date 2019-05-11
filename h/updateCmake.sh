#!/bin/bash
# TO Upgrade cmake to 3.13.4
which cmake
INSTALLED_VERS=`cmake --version | head -n 1`
# Download version
CMAKE_VERS="3.13.4"
CMAKE_VERS_S="cmake version 3.13.4"

extract () {
     if [ -f $1 ] ; then
         case $1 in
             *.tar.bz2)   tar xjf $1        ;;
             *.tar.gz)    tar xzf $1     ;;
             *.bz2)       bunzip2 $1       ;;
             *.rar)       rar x $1     ;;
             *.gz)        gunzip $1     ;;
             *.tar)       tar xf $1        ;;
             *.tbz2)      tar xjf $1      ;;
             *.tgz)       tar xzf $1       ;;
             *.zip)       unzip $1     ;;
             *.Z)         uncompress $1  ;;
             *.7z)        7z x $1    ;;
             *)           echo "'$1' cannot be extracted via extract()" ;;
         esac
     else
         echo "'$1' is not a valid file"
     fi
}



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
	if [ "$INSTALLED_VERS" != "$CMAKE_VERS_S" ]
	then

        if [[ "yes" == $(ask_yes_or_no "Uninstall  <$INSTALLED_VERS> and replace by <$CMAKE_VERS>?") ]]
         then
	 sudo apt-get remove cmake
	 sudo apt-get autoremove
	fi
	else
	echo "The correct version $INSTALLED_VERS is installed already!"
	exit 1
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
	
	if [[ "yes" == $(ask_yes_or_no "Remove installation files?") ]]   
	then 
	/bin/rm -rf ~/cmake-$CMAKE_VERS.tar.gz
	/bin/rm -rf ~/cmake-$CMAKE_VERS/
	fi

	
