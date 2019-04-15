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

#Creating Config-dir and
sudo mkdir /opt/controlpi
sudo ln  conf/logic.conf  /opt/controlpi/logic.conf
sudo chgrp www-data conf/logic.conf
sudo chmod g+rw     conf/logic.conf
sudo ln  conf/timers.conf /opt/controlpi/timers.conf
sudo ln conf/ControlPi.conf /opt/controlpi/ControlPi.conf
sudo chgrp www-data conf/timers.conf
sudo chmod g+rw     conf/timers.conf

sudo service ControlPi stop
sudo systemctl daemon-reload
sudo service ControlPi start
sudo service ControlPi enable
sudo service ControlPi status
