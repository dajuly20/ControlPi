<?php

if($_SERVER['REQUEST_METHOD'] == "GET"){

$file = file_get_contents("softLoigic.json");

echo $file;

}
else{
echo "Request method other than get!";


}

?>
