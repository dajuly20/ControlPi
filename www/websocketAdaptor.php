<?php

    require __DIR__ . '/vendor/autoload.php';

 $params = $_REQUEST["params"];
$size = count($params);

if(!$size)
	die("No Parameters defined. ?params[]=command1&params[]=command2 ...");

$i = 1;
    \Ratchet\Client\connect('ws://localhost:8080')->then(function($conn) {
        $conn->on('message', function($msg) use ($conn) {
	    echo "Reply [".$GLOBALS["i"]."]: {$msg}\n<br>";
	    if($GLOBALS["i"]++ >= $GLOBALS["size"]) $conn->close();
        });

	foreach($_REQUEST["params"] as $command){
        	$conn->send($command);
	}
    }, function ($e) {
        echo "Could not connect: {$e->getMessage()}\n";
    });

