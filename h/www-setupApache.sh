#!/bin/bash               
function ask_yes_or_no() {
    read -p "$1 ([y]es or [N]o): "
    case $(echo $REPLY | tr '[A-Z]' '[a-z]') in
        y|yes) echo "yes" ;;
        *)     echo "no" ;; 
    esac 
}

echo "edit www-apache2-config.cong first and edit Hostname/Alias fields in BOTH VHOSTS (HTTP & HTTPS)"
echo""
if [[ "no" == $(ask_yes_or_no "Hostname / Alias in www-apache2-config.conf correct?") ]]; then 
exit 1
fi

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


if [[ "no" == $(ask_yes_or_no "Install and execurte Certbot, to obtain a SSL-Certificate for this hostname?") ]]; then 
exit 1
fi

sudo sed -i "$ a\deb http://ftp.debian.org/debian stretch-backports main" /etc/apt/sources.list
sudo apt-get update
sudo apt-get install certbot python-certbot-apache -t stretch-backports -y --force-yes
