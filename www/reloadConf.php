<?php

$user  = shell_exec("whoami");
$owner = shell_exec('stat -c "%U" reloadConf');
if ($user != $owner){
echo "Please execute sudo chmod u+s reloadConf <br><br>";
}

$res = exec("./reloadConf >2&1;",$result_arr, $ret_val);

echo "$ret_val";

if($ret_val == 0){
echo "Successfully restarted! <br> $res";
}
else{
echo "restart Failed: <br> $res";
var_dump($result_arr);
}



?>
