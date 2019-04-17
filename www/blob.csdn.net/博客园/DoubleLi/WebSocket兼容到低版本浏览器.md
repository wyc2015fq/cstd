# WebSocket兼容到低版本浏览器 - DoubleLi - 博客园






就目前而言，WebSocket是最好的Web通信解决方案了。但是IE从10才开始兼容它，对于目前大量IE8存在的市场，原生的WebSocket显然不太实用，我们需要低版本兼容的解决方案。于是我模拟WebSocket在浏览器上的行为，用AS3写了个兼容的版本。 
内容有点多，这篇只把演示给搞定，至于AS的源码就丢到下一篇了。下面是效果图： 
![](http://img0.tuicool.com/3Y7zEn.png!web)
演示下载地址： [WebSocket兼容到低版本浏览器（演示实例）](http://www.web-tinker.com/files/WebSocket%E5%85%BC%E5%AE%B9%E5%88%B0%E4%BD%8E%E7%89%88%E6%9C%AC%E6%B5%8F%E8%A7%88%E5%99%A8.rar)
页面代码比较简单，用法和正常的WebSocket基本一样，只是引用了WebSocket.js这个文件来兼容低版本浏览器。当然还有个WebSocket.swf需要放在与页面相同的目录下。 

```
<script src="WebSocket.js"></script>
<script>
onload=function(){
  var ws=new WebSocket("ws://127.0.0.1:8000");
  ws.onopen=function(){
    //握手成功
    send.removeAttribute("disabled");
    send.value="发送";
  };
  ws.onmessage=function(e){
    //收到消息，放到页面上
    var dl,dt,dd,o=eval("("+e.data+")");
    dl=document.createElement("dl");
    dt=document.createElement("dt");
    dd=document.createElement("dd");
    dt.innerHTML=o.client;
    dd.appendChild(document.createTextNode(o.content));
    dl.appendChild(dt);
    dl.appendChild(dd);
    wall.appendChild(dl);
    wall.scrollTop=1E8; //滚动条移到底部
  };
  send.onclick=function(){
    //发送消息
    if(!message.value)return alert("请输入内容");
    ws.send(message.value);
    message.value="";
  };
};
</script>
<style>
dl {margin:3px 10px;font:14px/20px Consolas,微软雅黑;}
dt {color:blue;}
#wall {
  border:1px solid #CCC;width:200px;height:200px;
  overflow-y:auto;margin-bottom:10px;
}
</style>
<div id="wall"></div>
<input id="message" />
<input type="button" value="连接中···" id="send" disabled />
```
 服务器程序使用NodeJS代码，随手写的聊天室功能，仅仅为了实现功能而已，作为演示代码不要吐槽考虑的不够周全。里面的 [decodeDataFrame](http://www.web-tinker.com/article/20306.html) 和 [encodeDataFrame](http://www.web-tinker.com/article/20307.html) 在之前的文章中有出现过，这里就不贴出来了。由于需要兼容AS版，所以服务器程序需要比普通的WebSocket多一个AS的域安全验证的步骤。这是关键所在，如果使用现成的WebSocket库之类的东西可能得稍微修改源码才能兼容AS版。 

```
var crypto=require('crypto');
var fs=require('fs');
var WS='258EAFA5-E914-47DA-95CA-C5AB0DC85B11';

var pool=[]; //连接池

require('net').createServer(function(o){
  var key;
  o.on('data',function(e){
    if(!key){
      var data=e.toString();
      //Flash握手
      if(data=='<policy-file-request/>\0')
        return fs.readFile('policy.txt',function(err,data){
          o.write(data+'\0');
        });
      //WebSocket握手
      if(key=data.match(/Sec-WebSocket-Key: (.+)|$/)[1])
        key=crypto.createHash('sha1').update(key+WS).digest('base64'),
        o.write([
          'HTTP/1.1 101 Switching Protocols',
          'Upgrade: websocket',
          'Connection: Upgrade',
          'Sec-WebSocket-Accept: '+key
        ].join('\r\n')+'\r\n\r\n');
    }else{
      //解析数据
      var frame=decodeDataFrame(e);
      //文本帧
      if(frame.Opcode==1){
        //转义数据
        var content=frame.PayloadData.replace(/\W/g,function(e){
          e=e.charCodeAt(0).toString(16);
          if(e.length==3)e='0'+e;
          return '\\'+(e.length>2?'u':'x')+e;
        }),client=o.remoteAddress+":"+o.remotePort,buffer;
        //包装成JSON格式，并做成一个数据帧
        buffer=encodeDataFrame({
          FIN:1,Opcode:1,
          PayloadData:'{"client":"'+client+'","content":"'+content+'"}'
        });
        //对所有连接广播数据
        for(i=0;i<pool.length;i++)pool[i].write(buffer);
      };
    };
  });
  //断开时从连接池中移除
  o.on("close",function(){
    for(i=0;i<pool.length;i++)if(pool[i]==o)pool.splice(i,1);
  });
  //放入连接池中
  pool.push(o);
}).listen(8000);
```
 Flash握手就是AS的域安全验证，这个在之前的文章“ [使用AS直接与服务器TCP通信](http://www.web-tinker.com/article/20045.html) ”中有详细的说过，其实就是加载一个策略文件给它。把这个过程嵌入到WebSocket的握手之前就行。 
WebSocket.js的代码就不贴出来说明了。它负责构造一个类似原生WebSocket的接口的功能，里面会对不兼容WebSocket的浏览器加载WebSocket.swf来使之兼容。 
这个程序也是最近刚写的，存在漏洞在所难免，如果实用的话以后将慢慢完善它。









