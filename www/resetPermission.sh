#!/bin/bash
sudo chgrp -R  www-data .
sudo chmod -R g+rwX .
sudo chmod u+s reloadConf
