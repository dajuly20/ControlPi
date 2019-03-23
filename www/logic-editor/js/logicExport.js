
var lookup = [];

globDbg = true;


// Prompt to restore from localStorage
 function createInputPrompt() {
    $('#inputChooseDialog').empty();
    //var projectList = JSON.parse(localStorage.getItem("projectList"));
    //var flag = true;
   // for (id in projectList) {
   //     flag = false;
        $('#inputChooseDialog').append('<label class="option"><input type="radio" name="projectId" value="0" /> 0 </label>');
        $('#inputChooseDialog').append('<label class="option"><input type="radio" name="projectId" value="1" /> 1 </label>');
        $('#inputChooseDialog').append('<label class="option"><input type="radio" name="projectId" value="2" /> 2 </label>');
    //}
  //  if (flag) $('#inputChooseDialog').append('<p>Looks like no circuit has been saved yet. Create a new one and save it!</p>')
    $('#inputChooseDialog').dialog({
        width: "auto",
        buttons: [{
            text: "Hinzufügen",
            click: function() {
                if (!$("input[name=projectId]:checked").val()) return;
                load(JSON.parse(localStorage.getItem($("input[name=projectId]:checked").val())));
                $(this).dialog("close");
            },
        }]

    });
}


function saveExportedViaApi(data){
    
    $('.loadingIcon').fadeIn();
    
    $.ajax({
            url: '/logicUpdateApi.php?i=txt',
            type: 'POST',
            beforeSend: function(xhr) {
                xhr.setRequestHeader('X-CSRF-Token', $('meta[name="csrf-token"]').attr('content'))
            },
            data: {
                "data": data,
               // "id": logix_project_id,
                //"image": generateImageForOnline(),
                //name: projectName
            },
            success: function(response) {
                console.log(response);
                showMessage("Project: " + projectName + "  saved online.")
                $('.loadingIcon').fadeOut();
                //localStorage.removeItem("recover");
            },
            failure: function(err) {
                console.log("Error: "+err);
                showMessage("There was an error, we couldn't save to our servers")
                $('.loadingIcon').fadeOut();
            }
        });
    
}


 function createOutputPrompt(output) {
    $('#outputConfirmDialog').empty();
    //var projectList = JSON.parse(localStorage.getItem("projectList"));
    //var flag = true;
   // for (id in projectList) {
   //     flag = false;
        $('#outputConfirmDialog').append('<label class="option"><textarea rows="'+(output.length+7)+'"  cols="60" rows="10" name="softLogic">'+output.join('\n')+' </textarea></label>');
        
    //}
  //  if (flag) $('#inputChooseDialog').append('<p>Looks like no circuit has been saved yet. Create a new one and save it!</p>')
    $('#outputConfirmDialog').dialog({
        width: "700px",
        buttons: [{
            text: "Hinzufügen",
            click: function() {
                //if (!$("input[name=projectId]:checked").val()) return;
                //load(JSON.parse(localStorage.getItem($("input[name=projectId]:checked").val())));
                
                console.log($("textarea[name=softLogic]").val());
                
                saveExportedViaApi($("textarea[name=softLogic]").val());
                $(this).dialog("close");
            },
        }]

    });
}

function addToLookup(lookup, entities, oType, type){

   // e.g. AndGate gives an array of two of its kind.
   console.log(entities);
    for(key in entities){
   nodesOut = [];
   text ="";
   console.log("9999");
   console.log(typeof entities[key]);
   if(typeof  entities[key] != "object") continue;
    nodesIn  =  entities[key].customData.nodes.inp; 
    if(window.globDbg) console.log(">>>>>>"+key);
    if(nodesIn == undefined || nodesIn.length == 0){
    //if(oType == "NotGate"){ // Todo judz check for nodes.inp1 
    nodesIn = [ entities[key].customData.nodes.inp1 ];
    }
    switch(type){
 
    case "inp":
    text = entities[key].label;
    break;
    case "gate":
    
    case "static":
    break;
    default: 
    console.log("Type " + type + " not implemented");
    }
    nodeOut =  entities[key].customData.nodes.output1;
    lookup[nodeOut] = {nodesIn, text, oType,  type, nodeOut};
    }
}

function makeLookup(lookup, scope){
    console.log(">>>");
    console.log(scope);
    for(key in scope){
        O = scope[key];
        switch(key){
            case "AndGate":          addToLookup(lookup, O , key, "gate");     break;
            case "Ground":            addToLookup(lookup, O , key, "static");   break;
            case "Input":               addToLookup(lookup, O , key,  "inp");      break;
            case "NandGate":       addToLookup(lookup, O , key, "gate");    break;
            case "NorGate":         addToLookup(lookup, O , key, "gate");     break;
            case "NotGate":         addToLookup(lookup, O , key, "gate");     break;
            case "OrGate":           addToLookup(lookup, O , key, "gate");     break;
            case "Power":            addToLookup(lookup, O , key, "static");    break;
            case "XnorGate":       addToLookup(lookup, O , key, "gate"); break;
            case "XorGate":         addToLookup(lookup, O , key, "gate");  break;
        }
    }
}

function findNextNodes(outNode, lookup, allNodes, nodeBefore){
    var node = allNodes[outNode];
    var connectingNodes = JSON.parse(JSON.stringify( node.connections ));
   
  lengthbefore = connectingNodes.length;
        var index = connectingNodes.indexOf(nodeBefore[nodeBefore.length -2]);
        if (index !== -1) connectingNodes.splice(index, 1);
        lengthafter = connectingNodes.length;
   
    if(window.globDbg) console.log(connectingNodes);
   if(connectingNodes.length == 0){console.log("NO CONN NODES"); console.log(nodeBefore); }
    return connectingNodes;
}


function followXGate(outNodes,gateType, lookup, allNodes, gateNode){
        
var textPre = ""; 
var retText="";

switch(gateType){
        case "XnorGate":    retText +="!";   // fallthrough;  these both need special handeling.        (-1 so wont stop at this OUT )
        case "XorGate":      
        break;
        default: console.log(">Gatetype: "+gateType);    return -1;
        break;
}

if(outNodes.length <= 2){
  // XOR = (A |  B ) & !( A & B)   so just call followGate again and fake who we are ;) 
   var    orText   = followGate(outNodes, "OrGate",     lookup, allNodes, gateNode);
   if(orText == -1 || orText == -2) return orText; // If path failed, no need to go the other way aswell.
   var andText   = followGate(outNodes, "NandGate", lookup, allNodes, gateNode);
   
   
   console.log("Outnodeslen: "+outNodes.length);
   console.log("Or text: "+orText);
   console.log("andText"+andText);
   
   
   retText += " ( " + orText + " & " + andText + ")";
  }
  else{
  // Pop out first element... XOR with all other elements... until it's only two and above condition is true.
  var  A = followTree(outNodes.pop(), lookup, allNodes,[]);
  if(A == -1 || A == -2) return A;
  var  B  =  followXGate(outNodes,gateType, lookup, allNodes, gateNode);
  if(B == -1 || B == -2) return B;
  console.log("A part is : " + A);
  console.log("B part is : " + B);
  var XOR_manual = " ( " + A + " | " + B + " ) & ! ( " + A + " & " + B + " ) "; 
  return XOR_manual;
  }
    
    return retText;
    
       
    
}

function followGate(outNodes, gateType, lookup, allNodes, gateNode){
var gateNode = JSON.parse(JSON.stringify(gateNode));
var textPre = "";
var textMid = "";
switch(gateType){
        case "XnorGate":   // fallthrough;  these both need special handeling.        (-1 so wont stop at this OUT )
        case "XorGate":      return followXGate(outNodes, gateType, lookup, allNodes, gateNode);   
        break;
       
        case "NandGate":       
        case "NotGate":         textPre +="!";
        case "AndGate":         textPre+= "("; textMid = " & "; textEnd =")";
        break;
        
        case "NorGate":        textPre += "!"; 
        case "OrGate":          textPre += "("; textMid = " | "; textEnd = ")";
        break;   
        default: return -1;
        break;
    }
    
    var retText;
    
    retText =  JSON.parse(JSON.stringify(textPre));
    var i = 0;
    var t = 0;
    var u = 0;
    if(window.globDbg) console.log("GateType Entry: "+gateType);
    if(window.globDbg) console.log("OutNodes len "+outNodes.length);
    if(window.globDbg) console.log(outNodes);
    
    var oldNode;
    // If the GateNode is not in the lookup, it's a Wire Y - ing, so we have to keep the info where we come from.
    if( !lookup.hasOwnProperty(gateNode)){
        oldNode = gateNode;
    }
    else{
        // Otherwise we set it to empty array, to prevent it stopping at the next element which will be an Input of a gate.
        oldNode = [];
    }
    
    
    lastKey = "Ö";
    nextNodeKey = "unsinn";
    for(nextNodeKey in outNodes){
        console.log("ÖÖÖ ");
        console.log(typeof outNodes[nextNodeKey]);
        if(typeof outNodes[nextNodeKey] != "number") continue;
        
    var nextElement = -1;
    if(window.globDbg) console.log("Runs: "+(++u));
        if(lastKey == nextNodeKey){
        if(window.globDbg) console.log("LASTKEY "+lastKey+" == NEXT KEY "+nextNodeKey+" T= "+(t++));
        }
        else{
        if(window.globDbg) console.log("LASTKEY != NEXT KEY");
       // break;
        }
        
       
        nextElement = followTree(outNodes[nextNodeKey], lookup, allNodes,oldNode);
        if(window.globDbg) console.log("GateType: "+gateType+ " " +textMid+ " #"+gateNode);
        if(window.globDbg) console.log("NextElement: "+nextElement);
        if(window.globDbg) console.log("PreviousElement: "+previousElement);
        // If element is -1 then skip. 
        if(nextElement != -1 && nextElement != -2){
                console.log("Making entry "+i+"... adding "+nextElement+ " from outNode: " +outNodes[nextNodeKey]+ " to string content: "+retText);
                if(textMid ==""){console.log("EMPTY!!!");}
                if(i++ > 0 ) retText +=  JSON.parse(JSON.stringify(textMid)); 
                retText +=  JSON.parse(JSON.stringify(nextElement));
                var previousElement = nextElement;
        }
        else{
        
        console.log("Skipping next element "+ nextElement);
        console.log(outNodes);
        }
         
         lastKey=nextNodeKey;

    
    }
    
      retText +=  JSON.parse(JSON.stringify(textEnd));
      
      // Special cases to get rid of too many braces...
      // All elements were skipped... so no braces either... and return -1 because this ends in something bad else. 
      if(i == 0){
       return -1;
      }
      
      // One made it through.... so just return that one without braces. 
      // If there were more than one to make it we need the braces and concat signs (e.g. & ) anyway!
      else if(i == 1 && textPre !="!("){
        return previousElement;
      }
      
   if(window.globDbg)   console.log("Iteration retText is: "+retText);
   if(window.globDbg)  console.log("GateType: "+gateType);
   if(window.globDbg)  console.log("Returning :" +retText);
    return retText;

}



glob_i = 0;

function printType(type){
t_aus =0;
t_ein = 1;
t_ltg  = 2;

switch(type){
case t_aus: return "Ausgang"; 
case t_ein: return "Eingang";
case t_ltg: return "Leitung";

}
}

//                              Node to befin                        Stop at Output?
function followTree(outNode, lookup, allNodes, NODESBefore){
// Explicitly make a COPY of the array instead of working on the old one.
var nodeBefore = JSON.parse(JSON.stringify(NODESBefore));
t_aus =0;
t_ein = 1;
t_ltg  = 2;

node = allNodes[outNode];

if(window.globDbg) console.log("Stacksize: "+window.glob_i++);
if(window.glob_i >  550) return "-2"; // Remove? 

if(window.globDbg) console.log("##########################");
if(window.globDbg) console.log("Node: " +outNode);
if(window.globDbg) console.log(printType(node.type));
if(window.globDbg) console.log("Node before: "+nodeBefore[nodeBefore.length -1]);


// If this is not the entry point, and we reach an output, we're in a dead end.
if(nodeBefore.length > 0 && node.type == t_aus){
    if(window.globDbg) console.log("dead end");
    return -1;
}

if(nodeBefore.indexOf(outNode) != -1){
    console.log("Error: Circularity detected. Breaking the circle!");
    return -1;
}

nodeBefore.push(outNode); // Push this node to the stack of recent nodes.

nextNodes = -1;
ft = "nonsence";
switch(node.type){

// If it is a output, or a connection, go deeper in the rabbithole. 
case t_aus: case t_ltg:
     if(window.globDbg) console.log(">>>>>>>>Node Before: "+nodeBefore[nodeBefore.length -1]+ " OutNode: "+outNode+"   >>>>>>>>>");
      nextNodes = findNextNodes(outNode,lookup, allNodes, nodeBefore);
      
      if(nextNodes.length == 1){
        if(window.globDbg &&nextNodes[0] === undefined){ console.log("Next ~~~~~ Nodes[0] undefined: "); console.log(nextNodes);}
            
             if(window.globDbg) console.log(">>>Ret Message from thisNode: "+outNode+" nextNode "+nextNodes[0]+" is: "+ft);
             
             ft = followTree(nextNodes[0], lookup, allNodes, nodeBefore);
             if(ft == ""){ return -1;} 
             return ft;
     }
     
     else if(nextNodes.length > 1){
     // if we come here, the way goes in two directions... threting like "OR", so we're faking one and delegate to followGate ;-) 
       if(window.globDbg)  console.log("X X X ing in cirquit! "+outNode);
        return followGate(nextNodes, "OrGate", lookup, allNodes, nodeBefore);
     }
     
     else{
        // the Element is not connected ... dead end...
        if(window.globDbg) console.log("X X X dead end");
        return -1;
     }
 
break;

case t_ein: 
    // If it is a Input, find out if it's REALLY an input, or just the input of a gate.
    // All inputs have to be in the lookup Array. 
    lookupEntry  = lookup[outNode];
    
    if(window.globDbg)  console.log("Input");
    // If the type is input, we reached a endpoint.
    if(lookupEntry.type == "inp"){
       if(window.globDbg)  console.log("EXIT POINT");
       if(window.globDbg) console.log(lookupEntry.text);
       if(window.globDbg) console.log("^^^^^^^^^");
      
        return lookupEntry.text;
        }else if( lookupEntry.type == "static"){
         if(window.globDbg) console.log(" STATIC EXIT POINT!!!");
         if(window.globDbg) console.log(lookupEntry);
         if(window.globDbg) console.log("^^^^^^^^^");
         if(lookupEntry.oType == "Power") {
            return 1;
        }
        else{
            return "0";
        }
        }

    // When it is a gate we need to go down all paths (for each input of the gate) 
    else if(lookupEntry.type == "gate"){
       if(window.globDbg)  console.log("X X X typ gate");
        nodesIn = lookupEntry.nodesIn;
        return followGate(nodesIn, lookupEntry.oType, lookup, allNodes, outNode);  
    }

    else{
    console.log("Error: element not properly in lookup");
    }


break;
}

}



function logicExport(j){
    if(j == undefined) {
      j = JSON.parse(generateSaveData());  
    }
    
    
console.log(j); 
O = j.scopes[0];

outputs =   JSON.parse(JSON.stringify(j.scopes[0].Output));
allNodes = JSON.parse(JSON.stringify(j.scopes[0].allNodes));
console.log("§$§$§$§");
console.log(allNodes);

makeLookup(lookup, JSON.parse(JSON.stringify( j.scopes[0] ))  );


logicStrings = [];
u = 0;
// Now build Outputs :-) 
for (key in outputs){
if(typeof outputs[key] != "object") continue;
outName = outputs[key].label;
outNode  = outputs[key].customData.nodes.inp1;

console.log("LOLOL");
console.log(outNode);

allNodes = JSON.parse(JSON.stringify(allNodes));
lookup   = JSON.parse(JSON.stringify(lookup));

logicString = followTree(outNode, lookup, allNodes, []);
 if(window.globDbg) console.log("logic str: "+logicString);
completeLogic = outName + "=" + logicString;
logicStrings.push(completeLogic);

 if(window.globDbg) console.log("Completed "+completeLogic);
 if(window.globDbg) console.log("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
 if(window.globDbg) console.log("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

}


createOutputPrompt(logicStrings);
///showMessage(logicStrings.join("<br>"));
return completeLogic;

}





