<!DOCTYPE html>
<html >
<head>
  <meta charset="UTF-8">
  <title>ControlPi - Kleinsteuerung auf RaspberryPi</title>
  
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/normalize/5.0.0/normalize.min.css">
 <link rel="stylesheet" href="//code.jquery.com/ui/1.12.1/themes/base/jquery-ui.css">
  
  
      <link rel="stylesheet" href="css/style.css">

      
    <style>
  .nub, .leds, .switches, .titles{
  width: 50px;
  //border: 1px dotted black;
  display:inline-block;
  }

.headline{
text-align:center;
//border:1px solid black;
}  

  .preload{
display:none;
  }
  
  
  
  
  
  .switch_line input[type=text]
{
    margin-left: 5px;
	//margin-top:8px;
	font-size:10px;
	color:#545454;
	-moz-border-radius: 2px;
	-webkit-border-radius: 2px;
	-border-radius: 2px;
	display:none;
	width:140px;
	
}

.text_label
{
	float:left;
	margin-left: 5px;
	//margin-top:8px;
	font-size:10px;
	color:#545454;
	-moz-border-radius: 2px;
	-webkit-border-radius: 2px;
	-border-radius: 2px;
}

.edit
{
	float:left;
	//background:url(img/edit.png) no-repeat;
	width:32px;
	height:32px;
	display:block;
	cursor: pointer;
	margin-left:10px;
}

.clear
{
	clear:both;
	height:20px;
}


.switch_headline{
//border:1px solid red;
width:250px;
height:50px; 
display: block;
text-align:middle;
align-items: center;
vertical-align: middle;
}

.switch_headline p{
display: inline-block;
}


.switch_line{
//border:1px solid red;
width:250px;
height:50px; 
display: flex;
align-items: center;
vertical-align: middle;
}

.switch_line_title{
//border:1px solid red;
width:250px;
height:30px;
display: flex;
align-items: center;
vertical-align: middle;
}


.imgrep{
//border:1px solid blue;
width:50px;
}

.switch_line img{
float:left;
display:inline;
}

.switch_line p{
float:left;
display:inline;
font-weight: 400;
}


img.led-on{
content: url(img/led_on.png);
}

img.leds-off{
content: url(img/led_off.png);
}

img.switch-on{
content: url(img/button_on.png);
}

img.switch-off{
content: url(img/button_off.png);
}



#overview td{

}

  </style>
  <!--<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>-->
  <script src="https://code.jquery.com/jquery-1.12.4.js"></script>
  <script src="https://code.jquery.com/ui/1.12.1/jquery-ui.js"></script>
  <script type="text/javascript" src="js/ui.enhanced-alert.js"></script>
<!--  <script type="text/javascript" src="https://www.jqueryscript.net/demo/jQuery-UI-Based-Native-JS-Dialogs-Replacement-Enhanced-Alert/src/ui.enhanced-alert.js"></script>-->
  
  <script type="text/javascript" src="js/easy-editable-text.js"></script>
    <link type="text/css" rel="stylesheet" href="css/ui-enhanced-alert.css" />
    <script>

  var channels = null;
  var channels_parsed = null;
  
  
    var exp_public  = 2;
   var  exp_private = 1;
    var exp_none    = 0;
  
  
  function renderChannels(channels){
  $(".sel_row").html("");
    for (var i = 0; i < channels.length; i++) {
        var crumbs = channels[i].split(":");
        if(crumbs[2] == exp_public || (crumbs[2] == exp_private && crumbs[4] == "true")){
            var element = $("<option value='"+i+"'>"+crumbs[0]+" "+crumbs[1]+"</option>");
            element.data("crumbs", crumbs);
            $(".sel_row").append(element);
        }
   }
   
   // Loop through select's and let the n'th select element take the n'th entity.
   var selectors = $(".sel_row");
    for(var i=0; i < selectors.length; i++){
        var element = selectors.eq(i);
        var pos  = selectors.index(element);
        element.find("option").eq(pos).prop('selected',true);
        //console.log("Im the "+pos+" element");
    }
  
  }
    
    function switchToSSL(bla){
        if(bla){
         location.href = 'https:' + window.location.href.substring(window.location.protocol.length);
        }
    }
    
      function getLocation(href) {
    var l = document.createElement("a");
    l.href = href;
    return l;
    }
    

    function getStatus(){
        ws.send("show:status")
    }
    
    function getChannels(){
        ws.send("show:channels");
    }
    
  function openConnection(url, onmsg){
    ws = new WebSocket(url);
   
      ws.onopen = function(ev) {
        console.log("[connection opened]\n");
            getChannels();
            getStatus();
      };
      
       ws.onclose = function(ev) {
        console.log("[connection closed, trying reconnect] \n");
	  setTimeout(function(){openConnection();} ,1000);
	}
	
	ws.onmessage = onmsg;
      
      
  }
  
  function switch_clicked(event){
  if($(this).hasClass("switch-on") || $(this).hasClass("switch-off")){
    
    
    // Find line number and offset (left or right nub)
    var switch_line = $(this).parent();
    var switch_block = switch_line.parent();
    var line_number = switch_block.find(".switch_line").index(switch_line);
    var offset = $(this).parent().find("img").index($(this));
    
    // Find Value of corresponding <select>
    var selected_option = switch_block.find(".sel_row").eq(offset).find("option:selected");
    console.log(selected_option);
    var crumbs = selected_option.data("crumbs");
    
    
    
    
    var newVal = ( event.type == "mousedown") ? "high" : "low";
    var identifier = crumbs[0]+crumbs[1]+line_number;
    
    
    var command =    "set:"+identifier+":"+newVal;
    
    if(crumbs[3] == exp_public || (crumbs[3] == exp_private && crumbs[4] == "true")){
        ws.send(command);
    }
    else{
    $("#enhanced-confirm").css("width","320px");
    $.ea.prompt('Pleas enter token for Channel '+crumbs[0]+crumbs[1], 'Token required', function(r) {
     if (r) {
        ws.send("auth:"+crumbs[0]+":"+r);
     }
    });
   
    }
    console.log(line_number + " offset: "+offset);
    console.log(crumbs);
  }
  }
  
  function renderStatusupdate(status){
    
    // Loop for((each) over all <select>'s and get the selected <option>
     var selectedOptions = $(".sel_row option:selected");
     console.log(selectedOptions.length);
     
     selectedOptions.each( function(index){
           var switch_block = Math.floor(index / 2);  // e.g.   element 3 /2 = switch block1 (counting from 0)
           var switch_offset = index % 2;
           
            //console.log("eachSELECT "+index + " " + switch_block + " " +switch_offset );
            var crumbs =  $(this).data("crumbs");  // the element got data assigned earlier.
            var ch_en = crumbs[0]+crumbs[1]+""; // containing the crumbs of the channel status like H:i:2:0:false
            var bitval = status[ch_en]; // Contains all bits 0-7 (uint8_t) 
            
            var write_perm = crumbs[3];
            var authorized  = crumbs[4];
            
            var switch_line = $(".switch_block").eq(switch_block).find(".switch_line");
            
            var img_class;
            if((write_perm == exp_public) || write_perm == exp_private){
                img_class = "switch";
            }
            else{
                img_class = "led";
            }
            
            for(i=0; i<=7; i++){
                var nub =  switch_line.eq(i).find(".nub").eq(switch_offset);
                if( ((bitval >> i) & 1) == 1){
                // LED ON
                nub.removeClass();
                nub.addClass("nub "+img_class+"-on");
                }
                else{
                // LED OFF
                nub.removeClass();
                nub.addClass("nub "+img_class+"-off");
                }
            }
            
            
    });
   
    }
    
    
    
    var ws = null;
    
  $( function() {

  if(location.protocol != "https:"){
           $.ea.confirm("Your connection is unsafe!<br>Do you want to switch to SSL? ", 'Warning',switchToSSL,'alert');
           $("#enhanced-confirm").css("width","320px");
}
  

  $(".sel_row").change(function(){
  getStatus();
  });

  
  $(".nub").mousedown(switch_clicked);
  $(".nub").mouseup    (switch_clicked);
  
  
  var l = getLocation(location.href);
  var ws_protocol = (location.protocol == "https:") ? "wss://" : "ws://";
  var hostname    = l.hostname;
  var ws_url  = ws_protocol + hostname;
  console.log("Connecting to: "+ws_url);
  
   
   
   
       var onmessage = function(ev) {
      try{
        var obj = JSON.parse(ev.data);
        
        if(obj.channels != null){
            channels = obj.channels;
            renderChannels(channels);
        }
        else if(obj.status != null){
            renderStatusupdate(obj.status);
        }
        else if(obj.auth != null){
           if(obj.auth == "ok."){
                $.ea.alert("Token accepted.");
                getChannels();
            }
            else{
                $.ea.alert("Token incorrect.");
            }
        }
        
        //drawOutputs(obj.Ho,"H");
        //drawInputs(obj.Hi,"H");

        //drawOutputs(obj.Po,"P");
        //drawInputs(obj.Pi,"P");
      } catch(err) {
        console.log("Kein Json! "+err);
      } finally {
        console.log(ev.data);
      }
      };
   
  
   openConnection(ws_url, onmessage);   

  } );
  </script
  </head>


<body>


 
  <h1>ControlPi - Kleinsteuerung auf RaspberryPi</h1>

<div class="tabs">
  
  <input type="radio" id="tab1" name="tab-control" checked>
  <input type="radio" id="tab2" name="tab-control">
  <input type="radio" id="tab3" name="tab-control">  
  <input type="radio" id="tab4" name="tab-control">
  <ul>
    <li title="Features"><label for="tab1" role="button"><svg viewBox="0 0 24 24"><path d="M14,2A8,8 0 0,0 6,10A8,8 0 0,0 14,18A8,8 0 0,0 22,10H20C20,13.32 17.32,16 14,16A6,6 0 0,1 8,10A6,6 0 0,1 14,4C14.43,4 14.86,4.05 15.27,4.14L16.88,2.54C15.96,2.18 15,2 14,2M20.59,3.58L14,10.17L11.62,7.79L10.21,9.21L14,13L22,5M4.93,5.82C3.08,7.34 2,9.61 2,12A8,8 0 0,0 10,20C10.64,20 11.27,19.92 11.88,19.77C10.12,19.38 8.5,18.5 7.17,17.29C5.22,16.25 4,14.21 4,12C4,11.7 4.03,11.41 4.07,11.11C4.03,10.74 4,10.37 4,10C4,8.56 4.32,7.13 4.93,5.82Z"/>
</svg><br><span>Übersicht</span></label></li>
    <li title="Delivery Contents"><label for="tab2" role="button"><svg viewBox="0 0 24 24"><path d="M2,10.96C1.5,10.68 1.35,10.07 1.63,9.59L3.13,7C3.24,6.8 3.41,6.66 3.6,6.58L11.43,2.18C11.59,2.06 11.79,2 12,2C12.21,2 12.41,2.06 12.57,2.18L20.47,6.62C20.66,6.72 20.82,6.88 20.91,7.08L22.36,9.6C22.64,10.08 22.47,10.69 22,10.96L21,11.54V16.5C21,16.88 20.79,17.21 20.47,17.38L12.57,21.82C12.41,21.94 12.21,22 12,22C11.79,22 11.59,21.94 11.43,21.82L3.53,17.38C3.21,17.21 3,16.88 3,16.5V10.96C2.7,11.13 2.32,11.14 2,10.96M12,4.15V4.15L12,10.85V10.85L17.96,7.5L12,4.15M5,15.91L11,19.29V12.58L5,9.21V15.91M19,15.91V12.69L14,15.59C13.67,15.77 13.3,15.76 13,15.6V19.29L19,15.91M13.85,13.36L20.13,9.73L19.55,8.72L13.27,12.35L13.85,13.36Z" />
</svg><br><span>Steuerung ändern</span></label></li>
    <li title="Shipping"><label for="tab3" role="button"><svg viewBox="0 0 24 24">
    <path d="M3,4A2,2 0 0,0 1,6V17H3A3,3 0 0,0 6,20A3,3 0 0,0 9,17H15A3,3 0 0,0 18,20A3,3 0 0,0 21,17H23V12L20,8H17V4M10,6L14,10L10,14V11H4V9H10M17,9.5H19.5L21.47,12H17M6,15.5A1.5,1.5 0 0,1 7.5,17A1.5,1.5 0 0,1 6,18.5A1.5,1.5 0 0,1 4.5,17A1.5,1.5 0 0,1 6,15.5M18,15.5A1.5,1.5 0 0,1 19.5,17A1.5,1.5 0 0,1 18,18.5A1.5,1.5 0 0,1 16.5,17A1.5,1.5 0 0,1 18,15.5Z" />
</svg><br><span>Dienste</span></label></li>    <li title="Returns"><label for="tab4" role="button"><svg viewBox="0 0 24 24">
    <path d="M11,9H13V7H11M12,20C7.59,20 4,16.41 4,12C4,7.59 7.59,4 12,4C16.41,4 20,7.59 20,12C20,16.41 16.41,20 12,20M12,2A10,10 0 0,0 2,12A10,10 0 0,0 12,22A10,10 0 0,0 22,12A10,10 0 0,0 12,2M11,17H13V11H11V17Z" />
</svg><br><span>Hilfe</span></label></li>
  </ul>
  
  <div class="slider"><div class="indicator"></div></div>
  <div class="content">
    <section>
      <h2>Übersicht</h2>
     <div style="text-align: center">
        <img src="img/led_on.png"      class="preload">
        <img src="img/button_on.png" class="preload">
<table id="overview" border="0" style="" width="100%" style="table-layout:fixed;overflow:hidden">
<tr>

<td width="35%">

<div style="overflow:hidden;" style="background-color:blue">
    <div class="switch_block"><br>
        <div class="switch_headline">
            <div>Hardware Channels (Hi & Ho)</div>
        </div>


        <div class="switch_line_title" > 
            <p  class="imgrep" style=""><select class="sel_row">-</select></p>
            <p  class="imgrep" style=""><select class="sel_row">-</select></p>
        </div>

        <div class="switch_line" > 
            <img src="img/led_off.png" class="nub led-off" >
            <img src="img/led_off.png" class="nub led-off" >
            <p class="text_label"  >Beschreibung</p>
            <input  type="text" class="input_label" value="" />
        </div>

        <div class="switch_line" > 
            <img src="img/led_off.png" class="nub led-off" >
            <img src="img/led_off.png" class="nub led-off" >
            <p class="text_label"  >Beschreibung</p>
            <input  type="text" class="input_label" value="" />
        </div>

        <div class="switch_line" > 
            <img src="img/led_off.png" class="nub led-off" >
            <img src="img/led_off.png" class="nub led-off" >
            <p class="text_label"  >Beschreibung</p>
            <input  type="text" class="input_label" value="" />
        </div>

        <div class="switch_line" > 
            <img src="img/led_off.png" class="nub led-off" >
            <img src="img/led_off.png" class="nub led-off" >
            <p class="text_label"  >Beschreibung</p>
            <input  type="text" class="input_label" value="" />
        </div>

        <div class="switch_line" > 
            <img src="img/led_off.png" class="nub led-off" >
            <img src="img/led_off.png" class="nub led-off" >
            <p class="text_label"  >Beschreibung</p>
            <input  type="text" class="input_label" value="" />
        </div>

        <div class="switch_line" > 
            <img src="img/led_off.png" class="nub led-off" >
            <img src="img/led_off.png" class="nub led-off" >
            <p class="text_label"  >Beschreibung</p>
            <input  type="text" class="input_label" value="" />
        </div>

        <div class="switch_line" > 
            <img src="img/led_off.png" class="nub led-off" >
            <img src="img/led_off.png" class="nub led-off" >
            <p class="text_label"  >Beschreibung</p>
            <input  type="text" class="input_label" value="" />
        </div>

        <div class="switch_line" > 
            <img src="img/led_off.png" class="nub led-off" >
            <img src="img/led_off.png" class="nub led-off" >
            <p class="text_label"  >Beschreibung</p>
            <input  type="text" class="input_label" value="" />
        </div>

    </div>
</div>


</td>
<td witdh="30%">
  <img class="logicpreview" align="middle"  width="500px" src="img/logic.png"/>
</td>
<td width="35%">
<div style="overflow:hidden;" style="background-color:blue">
    <div class="switch_block"><br>
        <div class="switch_headline">
            <div>Hardware Channels (Hi & Ho)</div>
        </div>


        <div class="switch_line_title" > 
            <p  class="imgrep" style=""><select class="sel_row">-</select></p>
            <p  class="imgrep" style=""><select class="sel_row">-</select></p>
        </div>

        <div class="switch_line" > 
            <img src="img/led_off.png" class="nub led-off" >
            <img src="img/led_off.png" class="nub led-off" >
            <p class="text_label"  >Beschreibung</p>
            <input  type="text" class="input_label" value="" />
        </div>

        <div class="switch_line" > 
            <img src="img/led_off.png" class="nub led-off" >
            <img src="img/led_off.png" class="nub led-off" >
            <p class="text_label"  >Beschreibung</p>
            <input  type="text" class="input_label" value="" />
        </div>

        <div class="switch_line" > 
            <img src="img/led_off.png" class="nub led-off" >
            <img src="img/led_off.png" class="nub led-off" >
            <p class="text_label"  >Beschreibung</p>
            <input  type="text" class="input_label" value="" />
        </div>

        <div class="switch_line" > 
            <img src="img/led_off.png" class="nub led-off" >
            <img src="img/led_off.png" class="nub led-off" >
            <p class="text_label"  >Beschreibung</p>
            <input  type="text" class="input_label" value="" />
        </div>

        <div class="switch_line" > 
            <img src="img/led_off.png" class="nub led-off" >
            <img src="img/led_off.png" class="nub led-off" >
            <p class="text_label"  >Beschreibung</p>
            <input  type="text" class="input_label" value="" />
        </div>

        <div class="switch_line" > 
            <img src="img/led_off.png" class="nub led-off" >
            <img src="img/led_off.png" class="nub led-off" >
            <p class="text_label"  >Beschreibung</p>
            <input  type="text" class="input_label" value="" />
        </div>

        <div class="switch_line" > 
            <img src="img/led_off.png" class="nub led-off" >
            <img src="img/led_off.png" class="nub led-off" >
            <p class="text_label"  >Beschreibung</p>
            <input  type="text" class="input_label" value="" />
        </div>

        <div class="switch_line" > 
            <img src="img/led_off.png" class="nub led-off" >
            <img src="img/led_off.png" class="nub led-off" >
            <p class="text_label"  >Beschreibung</p>
            <input  type="text" class="input_label" value="" />
        </div>

    </div>
</div>

</td>
</tr>


  </table>
  
</div>
      
      <br>
    
         </section>
        <section>
          <h2>Steuerung ändern</h2>
         <iframe src="logic-editor/" width="100%" height="700" name="sprichwort_des_tages"></iframe>

         
         </section>
        <section>
          <h2>Dienste</h2>
          Dienste  ipsum dolor sit amet, consectetur adipisicing elit. Quam nemo ducimus eius, magnam error quisquam sunt voluptate labore, excepturi numquam! Alias libero optio sed harum debitis! Veniam, quia in eum.</section>
    <section>
          <h2>Hilfe</h2>
          HILFE ipsum dolor sit amet, consectetur adipisicing elit. Ipsa dicta vero rerum? Eaque repudiandae architecto libero reprehenderit aliquam magnam ratione quidem? Nobis doloribus molestiae enim deserunt necessitatibus eaque quidem incidunt.</section>
  </div>
</div>
  
  
</body>
</html>
