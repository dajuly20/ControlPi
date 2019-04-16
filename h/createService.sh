#!/bin/bash

#Create new systemuser controlpi
sudo useradd -G gpio,spi -r -s /bin/nologin controlpi
sudo chown controlpi ControlPi
sudo chown controlpi www/reloadConf
sudo chmod u+s www/reloadConf

#link to /usr/bin
sudo rm /usr/bin/ControlPi
sudo ln `realpath ControlPi` /usr/bin/ControlPi 
#Link service config 
sudo rm /etc/systemd/system/ControlPi.service
sudo ln h/ControlPi.service /etc/systemd/system/



rm /opt/controlpi/logic.conf
rm /opt/controlpi/timers.conf
rm /opt/controlpi/ControlPi.conf

#Creating Config-dir and
sudo mkdir /opt/controlpi
sudo ln  conf/logic.conf  /opt/controlpi/logic.conf
sudo chgrp www-data conf/logic.conf
sudo chmod g+rw     conf/logic.conf
sudo ln  conf/logic.json /opt/controlpi/logic.json
sudo chgrp www-data conf/logic.json
sudo chmod g+rw     conf/logic.json
sudo ln  conf/timers.conf /opt/controlpi/timers.conf
sudo ln conf/ControlPi.conf /opt/controlpi/ControlPi.conf
sudo chgrp www-data conf/timers.conf
sudo chmod g+rw     conf/timers.conf

sudo service ControlPi stop
sudo systemctl daemon-reload
sudo service ControlPi start
sudo systemctl enable ControlPi.service
sudo service ControlPi status


function ask_yes_or_no() {
    read -p "$1 ([y]es or [N]o): "
    case $(echo $REPLY | tr '[A-Z]' '[a-z]') in
        y|yes) echo "yes" ;;
        *)     echo "no" ;; 
    esac 
}

if [[ "yes" == $(ask_yes_or_no "Install&Setup Apache2?") ]]; then    
h/www-setupApache.sh
else
echo "Please execute h/www-setupApache.sh manually!"
fi

