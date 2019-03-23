<?php

$jsonfn = "/opt/controlpi/logic.json";
$txtfn  = "/opt/controlpi/logic.conf";

if($_SERVER['REQUEST_METHOD'] == "GET"){

//$file = file_get_contents("./logic.json",FALSE);
    if($_REQUEST["i"] == "json"){
        header('Content-Type: application/json');
        $file = file_get_contents($jsonfn, FALSE, NULL);
    }elseif($_REQUEST["i"] == "txt"){
        $file = file_get_contents($txtfn, FALSE, NULL);

    }else{
        echo "Specify parameter i with json or txt";
    }
    echo $file;

}
else{
echo "Method POST";
var_dump($_REQUEST);
if(!empty($_REQUEST["data"])){ // Un
echo "body not empty";

        $data = $_REQUEST["data"];

        if($_REQUEST["i"] == "json"){
		echo "writing $data to json... ";
		$fhandle = fopen($jsonfn,"w");
		fwrite($fhandle,$data);
        }elseif($_REQUEST["i"] == "txt"){
		echo "writing $data to txt";
		$fhandle = fopen($txtfn,"w");
		fwrite($fhandle,$data);
        }else{

        }

    }

}

?>
