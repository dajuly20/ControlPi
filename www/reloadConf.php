<?php

$user  = shell_exec('stat -c "%U" `which ControlPi`');
$owner = shell_exec('stat -c "%U" reloadConf');
if ($user != $owner){
echo "<h3>Error: ControlPi binary and reloadConf binary must be owned by the same user!</h3> <i>Please execute sudo chmod u+s reloadConf</i> <br>ControlPi binary owned by: $user<br>Reload Conf owned by: $owner<br><br>";
}

$res = exec("./reloadConf 2>&1",$result_arr, $ret_val);

if($ret_val == 0){
echo "Successfully restarted!\n'";
}
else{
echo "Restart Failed:\n";



echo "Output was:\n";
foreach($result_arr as $err){
    echo "$err \n";
}
}

?>
