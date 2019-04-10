#!/bin/bash
sudo usermod -aG systemd-journal www-dat
sudo service apache2 restart

sudo chgrp -R  www-data .
sudo chmod -R g+rwX .
sudo chmod u+s reloadConf
