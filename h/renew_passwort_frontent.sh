#!/bin/bash
echo "Please input new username and password for Logic-Editor"
read -p 'Username: ' user

if [ -z "$user" ]; then
echo "Username can not be empty"
exit 1
fi;


read -p 'New Password: ' pwd1

if [ -z "$pwd1" ]; then
echo "Password can not be empty"
exit 1
fi;

read -p 'Repeat Password: ' pwd2


if [ "$pwd1" != "$pwd2" ]; then
echo "Passwords do not match!"
fi

FC=`openssl passwd -crypt $pwd1`
printf "$user:$FC\n" > /opt/controlpi/.htpasswd
echo ".htaccess  file created successfully!"
