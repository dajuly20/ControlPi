# ControlPi

Turning your Rasperry Pi into a PLC

This Project was tested with a RaspberryPi2,3 and Zero with the [PiFaceDigital2](http://www.piface.org.uk/products/piface_digital_2/) HAT.
A executable is shipped which is built for Raspbian GNU/Linux 9 (stretch). 

## Getting Started
* Attach the PiFaceDigital2 to the RaspberryPi
* Open a ssh connection and clone this repository with `git clone https://github.com/dajuly20/ControlPi`
* cd into the new directory ControlPi and execute ./start_manual.sh to check if the executable works.
* When you have to build the project from source execute ./start_pull_and_build.sh to install dependencies and build.
* Otherwise you can create a Service for ControlPi by executing ./start_as_service.sh 


## First Control-Program
* Open your Browser and enter the IP / Hostname of your RaspberryPi e.g. [http://ControlPi3/]
  (If a error occurs try to execute h/www-setupApache.sh (which should have been executed by start_as_service.sh already.)
* Klick on tab Change PLC; Default username and password is "admin"
* Draw your PLC-Program 
* H input "H i" 0 - 3  have their own hardware buttons.
* H output "H o " 0 and 1 have their own relay. 
* Click Project -> Save&Export.
* Hit a Button you programmed and check out the result.


 
