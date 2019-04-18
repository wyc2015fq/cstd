# nodejs如何推送消息到浏览器（socket.io） - z69183787的专栏 - CSDN博客
2016年09月11日 18:54:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3683
了解下 `WebSocket` 协议及[socket.io](http://socket.io/)。nodejs和socket.io是不错的组合，对于不支持 `WebSocket` 协议的也做了轮询的fallback，使用很简单(来自于socket.io官网)：
后端：
```java
var io = require('socket.io').listen(80);
io.sockets.on('connection', function (socket) {
  socket.emit('news', { hello: 'world' });
  socket.on('my other event', function (data) {
    console.log(data);
  });
});
```
前端：
<scriptsrc="/socket.io/socket.io.js"></script><script>var socket = io.connect('http://localhost');
  socket.on('news', function(data){
    console.log(data);
    socket.emit('my other event', { my: 'data' });
  });
</script>
socket.io的简要介绍
> 
所有客户端都通过socket.io挂在nodejs服务器上(注意: 只是挂着，不需要任何循环，因为它是事件驱动的)；需要推送消息了，服务器就与nodejs通信(比如访问某个地址来实现)，告诉它推送什么消息到哪里；nodejs收到推送信号后，则通过socket.io实时传输数据给浏览器。这个其实也是一条单向的路，因为nodejs服务器不具备与php通信的能力，实际上也不需要，网页上直接连php就可以了。
可以参考这篇文章:[PHP ServerPush (推送) 技术的探讨](http://www.cnblogs.com/hnrainll/archive/2013/05/07/3064874.html)
**接下来开始整理下思路**
1.正如简要介绍中所说的首先要将客户端都通过socket.io挂在nodejs服务器上.
在用户进入拍卖页面后开始连接socket.io ，然后将当前客户端的'用户id','拍卖id','当前最高价','socket.id'存储至node.js全局变量socketUser中.
//客户端
> 
var socket = io.connect("http://demo.xiaocai.name":339");
socket.on('conn', function (data) {
  var postdata = {
'c_id'   : PageValue.charinfo.c_id,     //用户id
        'c_name' : PageValue.charinfo.c_name,   //用户昵称
        'guid'   : PageValue.infodata.id,//拍卖id
        'price'  : PageValue.infodata.max_price,//当前最高价
  }
  socket.emit('login', postdata,function(result){
     console.log('登陆成功');
  });
});
//服务端
> 
var   sio     = require('socket.io');
var   express = require('express');
var   app  =  module.export = express.createServer();
//初始化
var socketUser = {};
io  = sio.listen(app);
io.set('log level', 1);//将socket.io中的debug信息关闭
//监听连接
io.sockets.on('connection', function (socket){
    //响应连接
    io.sockets.emit('conn', { text: 'socketId:'+socket.id});
    //监听用户登录并存储socket
    socket.on('login', function (data,fn) {
socketUser[socket.id] = {'c_id':data.c_id,'guid':data.guid,'price':data.price,'socket':socket};
    });
   //监听断线
    socket.on('disconnect', function(){
        console.log('-链接断开['+socket.id+']-');
        delete socketUser[socket.id];
    });
});
2.需要推送消息时服务器就与nodejs通信告诉它推送什么消息到哪里
当有用户出价时最高价格将发生改变,这时候通过socket.io来监听这个动作，接着向挂在nodejs服务器上的socket客户端推送数据.
前端js:某个客户端用户给出了新的价格时发出请求‘postprice’并带上当前拍卖的id
> 
socket.emit('postprice', {'guid':PageValue.infodata.id});
服务端nodejs:服务端监听请求postprice并执行pushprice方法向其它客户端推送消息
> 
socket.on('postprice', function (data,fn) {
    console.log('-用户出价['+data.guid+']-');
    pushprice(data.guid);
});
从redis缓存中读取出当前拍卖的最高价,然后遍历该拍卖下的socketUser集合,若它的价格低于取出的最高价则向它推送最新的价格(并且更新它的最高价).GetRedisKey是个读取reids的方法该方法在底部贴出.
> 
    var pushprice = function(guid){
        console.log('-推送数据['+guid+']-');
        common_func.GetRedisKey("AuctionMaxPrice-"+guid,function(val){
            if(!val){
                return false;
            }
            for(var values in socketUser){
                if(  parseFloat(socketUser[values].price) < val && socketUser[values].guid == guid ){
                    socketUser[values].socket.emit('receive',{'nowprice':val});
                    socketUser[values].price = val;
                }
            }
        });
    }
3.客户端接受推送的数据
> 
socket.on('receive',function(maxprice){
    $('#NowUserTxt').html('￥'+maxprice);
});
GetRedisKey是公共函数中获得redis缓存的方法,这边单独贴出来
> 
exports.GetRedisKey = function(key,fun){
    if( typeof redis_client == 'undefined' ){
        var redis      = require("redis");
        redis_client   = redis.createClient(RedisPort,RedisHost);
        redis_client.on("error", function (err) {
            common_func.insertlog("Error(redis): " + err);
        })
    }
    redis_client.get(key, function (err, reply) {
        if(reply){
            fun(reply.toString());
        }else{
            fun(false);
            common_func.insertlog('Error(redis): get('+key+') not data');
        }
    });
}
