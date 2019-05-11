<?php

$jsonfn = "/opt/controlpi/logic.json";
$txtfn  = "/opt/controlpi/logic.conf";
$timerfn = "/opt/controlpi/timers.conf";

function saveImg($base64){

$path = "/opt/controlpi/";
$path2 = "./img/";
$fn     = "logic";

//$base64 = "data:image/png;base64,gAAAQ8AAAC6CAMAAACHgTh+AA=";

if(preg_match("/^data:image\/(?<extension>(?:png|gif|jpg|jpeg));base64,(?<image>.+)$/", $base64, $matchings))
{
   $imageData = base64_decode($matchings['image']);
   $extension = $matchings['extension'];
   $filename   = sprintf("%s%s.%s",$path,   $fn, $extension);
   $filename2 = sprintf("%s%s.%s",$path2, $fn, $extension);
   if(file_put_contents($filename, $imageData) && file_put_contents($filename2, $imageData))
   {
      // image saved
      echo "Image saved to $filename!";
   }
}
else{
$error = preg_last_error();
echo "Error: Image does not match a extension <$error>";

}

}

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
        $timers = "";
        if(isset($_REQUEST["timers"])){
            $timers = $_REQUEST["timers"];
        }
        
        if(!empty($_REQUEST["image"])){
            saveImg($_REQUEST["image"]);
        }
        
        if($_REQUEST["i"] == "json"){
		echo "writing $data to json... ";
		$fhandle = fopen($jsonfn,"w");
		fwrite($fhandle,$data);
        }elseif($_REQUEST["i"] == "txt"){
		echo "writing $data to txt";
		$fhandle = fopen($txtfn,"w");
		fwrite($fhandle,$data);
		
		$fhandle2 = fopen($timerfn,"w");
		fwrite($fhandle2,$timers);
		
        }else{

        }

    }

}

?>
