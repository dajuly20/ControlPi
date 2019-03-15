/*
   POSSIBLE SECURITY RISK
   better not owned by root

   sends USRSIG1 to the Process of ControlPi which will make it reload.
*/

// Compiles to reloadCond 
// give this executable the sticky bit with chmod u+s, what will make it executable  under the user that it owns.
// enabling e.g. www-data to  restart the backend, evenn though the process is owned by someone else. 
// so just make sure this script and the main executable are owned by the same user. Also the executable-name must match!

// Comile with gcc reloadConf.c -o reloadConf
// Then sudo chmod u+g reloadConf
#include <stdlib.h>
int main(){
system("kill -10 `pidof ControlPi`");
}
