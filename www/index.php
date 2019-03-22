
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8" />
  <title>WebSocket Chat - CppCon2018</title>
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
  <style>
  .leds, .switches, .titles{
  width: 50px;
  border: 1px dotted black;
  display:inline-block;
  }

.headline{
width: 450px;
text-align:center;
border:1px solid black;
}  

  .preload{
display:none;
  }
  </style>
</head>
<body>
  <h1>ControlPi Interface</h1>
  
<img src="img/led_on.png" class="preload">
<img src="img/button_on.png" class="preload"><br>
<div id="H"><br>
<div class="headline">Hardware Channels (Hi & Ho)</div><br>
    <div class="titles">0</div>
    <div class="titles">1</div>
    <div class="titles">2</div>
    <div class="titles">3</div>
    <div class="titles">4</div>
    <div class="titles">5</div>
    <div class="titles">6</div>
    <div class="titles">7</div>
    <br>
    <img src="img/led_off.png" class="leds">
    <img src="img/led_off.png" class="leds">
    <img src="img/led_off.png" class="leds">
    <img src="img/led_off.png" class="leds">
    <img src="img/led_off.png" class="leds">
    <img src="img/led_off.png" class="leds">
    <img src="img/led_off.png" class="leds">
    <img src="img/led_off.png" class="leds">
    <br>

    <img src="img/button_off.png" class="switches">
    <img src="img/button_off.png" class="switches">
    <img src="img/button_off.png" class="switches">
    <img src="img/button_off.png" class="switches">
    <img src="img/button_off.png" class="switches">
    <img src="img/button_off.png" class="switches">
    <img src="img/button_off.png" class="switches">
    <img src="img/button_off.png" class="switches">
<br>
</div>
<br><br>
<div id="P"><br>
<div class="headline">Pipe (Pi und Po)</div><br>
    <div class="titles">0</div>
    <div class="titles">1</div>
    <div class="titles">2</div>
    <div class="titles">3</div>
    <div class="titles">4</div>
    <div class="titles">5</div>
    <div class="titles">6</div>
    <div class="titles">7</div>
    <br>
    <img src="img/led_off.png" class="leds">
    <img src="img/led_off.png" class="leds">
    <img src="img/led_off.png" class="leds">
    <img src="img/led_off.png" class="leds">
    <img src="img/led_off.png" class="leds">
    <img src="img/led_off.png" class="leds">
    <img src="img/led_off.png" class="leds">
    <img src="img/led_off.png" class="leds">
    <br>

    <img src="img/button_off.png" class="switches">
    <img src="img/button_off.png" class="switches">
    <img src="img/button_off.png" class="switches">
    <img src="img/button_off.png" class="switches">
    <img src="img/button_off.png" class="switches">
    <img src="img/button_off.png" class="switches">
    <img src="img/button_off.png" class="switches">
    <img src="img/button_off.png" class="switches">
<br>
</div>

<h3><?php echo "PHP ready!";?> 
  Server URI: <input class="draw-border" id="uri" size="47" value="wss://<?php echo $_SERVER["HTTP_HOST"];?>" style="margin-bottom: 5px;">
  <button class="echo-button" id="connect">Connect</button>
  <button class="echo-button" id="disconnect">Disconnect</button><br>
  

  <pre id="messages" style="width: 600px; height: 400px; border: solid 1px #cccccc; margin-bottom: 5px; overflow: scroll;"></pre>

  
  <div style="margin-bottom: 5px;">
    Message<br>
    <input class="draw-border" id="sendMessage" size="74" value="set:Pi0:high">
    <button class="echo-button" id="send">Send</button>
  </div>
  <script>
    var ws = null;
    
    function appendIt(msg){
     
	// allow 1px inaccuracy by adding 1
	var isScrolledToBottom = messages.scrollHeight - messages.clientHeight <= messages.scrollTop + 1;
    	
    	var newElement = document.createElement("div");
	newElement.innerHTML = msg;
    	messages.appendChild(newElement);
    	// scroll to bottom if isScrolledToBotto
    	if(isScrolledToBottom)
      		messages.scrollTop = messages.scrollHeight - messages.clientHeight;

    }
    
    function drawOutputs(o,id){
    
        for(i=0; i<=7; i++){
            if( ((o >> i) & 1) == 1){
            $("#"+id+" .leds").eq(i).attr("src","img/led_on.png");
            }
            else{
            $("#"+id+" .leds").eq(i).attr("src","img/led_off.png");
            }
        }
    }
    
       function drawInputs(o,id){
    
        for(i=0; i<=7; i++){
            if( ((o >> i) & 1) == 1){
            $("#"+id+" .switches").eq(i).attr("src","img/button_on.png");
            }
            else{
            $("#"+id+" .switches").eq(i).attr("src","img/button_off.png");
            }
        }
    }
    
    connect.onclick = function() {
      ws = new WebSocket(uri.value);
      ws.onopen = function(ev) {
        appendIt("[connection opened]\n");
      };
      ws.onclose = function(ev) {
        appendIt("[connection closed, trying reconnect] \n");
	setTimeout(function(){connect.click()} ,1000)
      };
      
      var c = 0;
      ws.onmessage = function(ev) {
      try{
      var obj = JSON.parse(ev.data);
      console.log(obj.Ho);
      drawOutputs(obj.Ho,"H");
      drawInputs(obj.Hi,"H");

      drawOutputs(obj.Po,"P");
      drawInputs(obj.Pi,"P");
      } catch(err) {
      console.log("Kein Json!");
      } finally {
        appendIt(ev.data);
       }
      };
      ws.onerror = function(ev) {
        appendIt( "[error]\n");
        console.log(ev);
      };
    };
    disconnect.onclick = function() {
      ws.close();
    };
    send.onclick = function() {
      ws.send(sendMessage.value);
      sendMessage.value = "";
    };
    sendMessage.onkeyup = function(ev) {
      ev.preventDefault();
      if (event.keyCode === 13) {
        send.click();
      }
    }


$(function() {
$(".switches").mousedown(function(event){ 
    var el = $(event.target);
    var par_id =  el.parent().attr('id');

    var pos = $("#"+par_id+" .switches").index(el);
     console.log("Triggering: "+par_id+"i"+pos+"=1");
    var identifier = par_id+"i"+pos;
    
    var sendStr = "set:"+identifier+":high";
    ws.send(sendStr);
    
});

$(".switches").mouseup(function(event){ 
    var el = $(event.target);
    var par_id =  el.parent().attr('id');

    var pos = $("#"+par_id+" .switches").index(el);
     console.log("Triggering: "+par_id+"i"+pos+"=0");
    var identifier = par_id+"i"+pos;
    var sendStr = "set:"+identifier+":low";
    ws.send(sendStr);
});

});

  </script>

  
  <ifame src="./logic-editor"></iframe>
<iframe src="logic-editor/" 
width="60%" height="500" name="sprichwort_des_tages">
</iframe>
  </body>
</html>
