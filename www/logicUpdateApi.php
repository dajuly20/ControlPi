<?php

if($_SERVER['REQUEST_METHOD'] == "GET"){

//$file = file_get_contents("./logic.json",FALSE);
if($_REQUEST["i"] == "json"){
$file = file_get_contents('./logic.json', FALSE, NULL);
}elseif($_REQUEST["i"] == "txt"){
$file = file_get_contents('./logic.conf', FALSE, NULL);

}else{
echo "Specify parameter i with json or txt";
}

echo $file;

}
else{
echo "Request method other than get!";


}

?>
