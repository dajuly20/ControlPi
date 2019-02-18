#!/bin/bash               
function ask_yes_or_no() {
    read -p "$1 ([y]es or [N]o): "
    case $(echo $REPLY | tr '[A-Z]' '[a-z]') in
        y|yes) echo "yes" ;;
        *)     echo "no" ;; 
    esac 
}

if [ -z `which apache2` ]
then
echo "Apache2 not installed."
if [[ "no" == $(ask_yes_or_no "Install Apache2?") ]]; then    
exit 1
fi

sudo apt-get update
sudo apt-get install apache2 
fi

if [ -z `which php` ]; then
if [[ "no" == $(ask_yes_or_no "Install PHP") ]]; then    
exit 1
fi
sudo apt-get install php-pear php-fpm php-dev php-zip php-curl php-xmlrpc php-gd php-mysql php-mbstring php-xml libapache2-mod-php
fi


if [ -z `which composer` ]; then
echo "Composer not found."
if [[ "no" == $(ask_yes_or_no "Install Composer") ]]; then    
exit 1
fi
h/installComposer.sh
fi

echo "Enabling necessary Apache2 modules..."
echo ""
sudo a2enmod ssl
sudo a2enmod rewrite
sudo a2enmod proxy
#sudo a2enmod proxy_http
sudo a2enmod proxy_wstunnel
sudo service apache2 restart

# Linking WWW dir to Apache Webroot
sudo ln -s `realpath ./www` /var/www/controlPi 
echo ""
echo ""
echo "Modules set-up. Import config file for apache"
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
#this is only a link to sites-avail... 
sudo rm /etc/apache2/sites-enabled/000-default.conf 
sudo service apache2 restart

echo ""
echo "Finally downloading PHP dependencies for PHP-WebSocket Adaptor"
cd www
./init.sh
echo "DONE"
echo ""

if [[ "no" == $(ask_yes_or_no "Install and execurte Certbot, to obtain a SSL-Certificate for this hostname?") ]]; then 
exit 1
fi

sudo sed -i "$ a\deb http://ftp.debian.org/debian stretch-backports main" /etc/apt/sources.list
sudo apt-get update
sudo apt-get install certbot python-certbot-apache -t stretch-backports -y --force-yes
