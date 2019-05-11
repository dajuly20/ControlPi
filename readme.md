# ControlPi

Turning your Rasperry Pi into a PLC

This Project was tested with a RaspberryPi2,3 and Zero with the [PiFaceDigital2](http://www.piface.org.uk/products/piface_digital_2/) HAT.
A executable is shipped which is built for Raspbian GNU/Linux 9 (stretch). 
See Demo-Video on [youtube.com](https://www.youtube.com/watch?v=_jfn4xiY1Rk)
See Documentation (german) https://github.com/dajuly20/Thesis/blob/master/thesis.pdf

[![Demo on YouTume](https://media.giphy.com/media/QB3UTSvPs2DX91s0Ya/giphy.gif)](https://www.youtube.com/watch?v=_jfn4xiY1Rk)


## Easiest way:
* Attach the PiFaceDigital2 to the RaspberryPi
* Download full SD Image: wget https://owncloud.wiche.eu/download/ControlPi/ControlPi-Raspbian-Strech.tar.gz
* Extract with `tar -xvzf ControlPi-Raspbian-Strech.tar.gz`
* opt: `sha1sum ControlPi-Raspbian-Strech.img` and check if equal to `cat ControlPi-Raspbian-Strech.img.sha1sum`
* Transfer to SD Card with `dd if=ControlPi-Raspbian-Strech.img of=/dev/<Your-SD-Card> bs=4M status=progress`


## If you have Raspbian installed already
* Attach the PiFaceDigital2 to the RaspberryPi
* Open a ssh connection and clone this repository with `git clone https://github.com/dajuly20/ControlPi`
* cd into the new directory ControlPi and execute `./start_manual.sh` to check if the executable works.
* When you have to build the project from source execute `./start_pull_and_build.sh` to install dependencies and build.
* Otherwise you can create a Service for ControlPi by executing `./start_as_service.sh`


## First Control-Program
* Open your Browser and enter the IP / Hostname of your RaspberryPi e.g. [http://ControlPi3/]
  (If a error occurs try to execute `h/www-setupApache.sh` (which should have been executed by start_as_service.sh already.)
* Klick on tab Change PLC; Default username and password is "admin"  (Execute `h/renew_passwort_frontent.sh`in Project folder)
* Draw your PLC-Program 
* H input "H i" 0 - 3  have their own hardware buttons.
* H output "H o " 0 and 1 have their own relay. 
* Click Project -> Save&Export.
* Hit a Button you programmed and check out the result.


 
