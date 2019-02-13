#!/bin/bash
#change the group of www folder to www-data.
sudo chgrp -R www-data www/
# and make anything group-readable.
sudo chmod -R ug+rwX www/
# and make all folders Browsable (notice the capital X) 
#sudo chmod -R ug+X www/ 
#Let newly created files inherit the group "www-data" from the parent folder.
sudo chmod g+s www
sudo ln h/www-apache2-config.conf /etc/apache2/sites-enabled/controlpi.conf
sudo service apache2 restart

