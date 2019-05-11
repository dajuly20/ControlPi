<?php

    require __DIR__ . '/vendor/autoload.php';

    
     if(!isset($argv[1]) || empty($argv[1]))
	die("Usage: $argv[0] ws://example.com/ws/\n");

 $remoteAddr = $argv[1];

/*
Runs on Localhost (ControlPi2 at the moment)
Connects to ControlPi3 

*/

 function err($e){
        echo "Could not connect: {$e->getMessage()}\n";
    }
    
    
 

echo "Using remote Addr: $remoteAddr";

	
$connLocal     = false;
$connRemote = false;

  \Ratchet\Client\connect('ws://localhost:8080')->then(function($conn) {
        $GLOBALS["connLocal"] = $conn;
        $connRemote = $GLOBALS["connRemote"];
        $connLocal     = $conn;
        $conn->on('message', function($msg) use ($connLocal, $connRemote) {
            echo "Received local: {$msg}\n";
            
        });
        
        $conn->send('Hello World!');
    }, function ($e) {
        echo "Could not connect: {$e->getMessage()}\n";
    });
    
    echo "test";
    
    
      \Ratchet\Client\connect($remoteAddr)->then(function($conn) {
        $GLOBALS["connRemote"] = $conn;
        $connLocal = $GLOBALS["connLocal"];
        $connRemote = $conn;
        echo "IN Remote";
        $conn->on('message', function($msg) use ($connLocal, $connRemote) {
            echo "Received remote: {$msg}\n";
            
        });
        echo "in Remote2";
        $conn->send('Hello World!');
    }, function ($e) {
        echo "Could not connect: {$e->getMessage()}\n";
    });
    
    
