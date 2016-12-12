//print("Hello V8");

function trace(){
  
    var str = "";
    for(var i = 0;i<arguments.length;i++){
        str += arguments[i] + ","
        
    }
    
    print(str);
}

//trace("Hello","World");


load('app1.js');
