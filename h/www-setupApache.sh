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
sudo rm /var/www/controlPi
sudo ln -s `realpath ./www` /var/www/controlPi 

#change the group of www folder to www-data.
sudo chgrp -R www-data www/
# and make anything group-readable.
sudo chmod -R ug+rwX www/
# and make all folders Browsable (notice the capital X) 
#sudo chmod -R ug+X www/ 
#Let newly created files inherit the group "www-data" from the parent folder.
sudo chmod g+s www
# Hardlink this Apache-Config to the Apache config folger
sudo ln h/www-apache2-config.conf /etc/apache2/sites-enabled/controlpi.conf
#this is only a link to sites-avail... 
sudo rm /etc/apache2/sites-enabled/000-default.conf 



echo ""
echo ""
echo "Modules set-up. Import config file for apache"
echo ""
echo "Do you want to obtain a valid SSL Certificate using CertBot? "
echo "REQUIRES: Fully qualified domain name e.g. ControlPi.yourDynDny.com that is forwarded on this mashine on both ports 80 and 443"
echo ""
USE_CB="no"
if [[ "yes" == $(ask_yes_or_no "Use Certbot to obtain a Certificate?") ]]; then 
if [[ "yes" == $(ask_yes_or_no "Is this mashine externally reachable?") ]]; then 
USE_CB="yes"
echo "Please check www-apache2-config.conf for correct Hostname/Alias fields in BOTH VHOSTS (HTTP & HTTPS)"
fi
fi

if [[ "yes" == $(ask_yes_or_no "View / Edit Apache Config now?") ]]; then 
nano h/www-apache2-config.conf
fi



if [[ "yes" == $USE_CB ]]; then

sudo sed -i "$ a\deb http://ftp.debian.org/debian stretch-backports main" /etc/apt/sources.list
sudo apt-get update
sudo apt-get install certbot python-certbot-apache -t stretch-backports -y --force-yes
sudo certbot --apache
else
echo "Creating Self-signed certificate >apache-selfsigned<"
sudo openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout /etc/ssl/private/apache-selfsigned.key -out /etc/ssl/certs/apache-selfsigned.crt
fi





sudo service apache2 restart

echo ""
echo "Finally downloading PHP dependencies for PHP-WebSocket Adaptor"
cd www
./init.sh
cd -
echo "DONE"
echo ""

