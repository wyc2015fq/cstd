# php实现Web消息推送框架 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年09月18日 15:38:54[boonya](https://me.csdn.net/boonya)阅读数：6185










文章来源：[http://www.workerman.net/web-sender](http://www.workerman.net/web-sender)


web-msg-sender是一款web长连接推送框架，采用PHPSocket.IO开发，基于WebSocket长连接通讯，如果浏览器不支持WebSocket则自动转用comet推送。通过后台推送消息，消息可以即时推送到客户端，非轮询，实时性非常好，性能很高。

**特点：**
- 多浏览器支持
- 支持针对单个用户推送消息
- 支持向所有用户推送消息
- 长连接推送（websocket或者comet），消息即时到达
- 支持在线用户数实时统计展示
- 支持在线页面数实时统计展示
- 支持跨域推送


### 下载



#### WEB消息推送框架

Fork GItHub:[https://github.com/walkor/web-msg-sender/fork](https://github.com/walkor/web-msg-sender/fork)



#### Windows版本下载


**Demo1:**

接收消息页面：[http://www.workerman.net:2123/](http://www.workerman.net:2123/)

推送接口url：[http://www.workerman.net:2121/?type=publish&to=uid&content=content](http://www.workerman.net:2121/?type=publish&to=&content=content)

其中to为接收消息的uid，如果传空则向所有人推送消息content 为消息内容

**Demo2:**

见本页面页脚在线统计部分，支持实时的在线人数和在线页面统计



### 其它应用

[vmstat服务器状态监控服务](http://www.workerman.net/workerman-vmstat)
WEB消息推送框架
[BrowserQuest PHP版本](http://www.workerman.net/browserquest)[摄像头视频直播](http://www.workerman.net/camera)[Thrift
 RPC 远程调用框架](http://www.workerman.net/workerman-thrift)[Json RPC远程调用框架](http://www.workerman.net/workerman-jsonrpc)[聊天室框架](http://www.workerman.net/workerman-chat)[统计监控系统](http://www.workerman.net/workerman-statistics)[PHP小蝌蚪实时交互游戏  hot](http://www.workerman.net/workerman-todpole)[flappy
 bird 多人在线版游戏](http://www.workerman.net/workerman-flappybird)





### 安装启动

1、下载：选择版本并下载([linux版本](http://www.workerman.net/download/senderzip)、[windows版本](https://github.com/walkor/web-msg-sender-for-win/archive/master.zip))web-msg-sender，并解压缩到任意目录

2、启动：linux系统cd到到框架目录里运行`php start.php start -d`。windows系统进入到框架目录里双击start_for_win.bat文件启动。

3、测试：浏览器访问端口http://ip:2123或者http://域名:2123，例如[http://workerman.net:2123](http://workerman.net:2123/)如图：(如果无法访问请检查服务器防火墙)
![WEB推送使用界面](http://www.workerman.net/img/web-msg-sender-demo.png)

### 前端测试：

支持跨域推送，开发者可以不用建立服务端，直接使用线上的推送服务测试，只要引入js文件并设置下端口及回调即可，例如在任意站点中加入如下代码即可收到消息并统计数据：
<script src='http://cdn.bootcss.com/socket.io/1.3.7/socket.io.js'></script>
<script>
    // 连接服务端
    var socket = io('http://workerman.net:2120');
    // uid可以是自己网站的用户id，以便针对uid推送以及统计在线人数
    uid = 123;
    // socket连接后以uid登录
    socket.on('connect', function(){
    	socket.emit('login', uid);
    });
    // 后端推送来消息时
    socket.on('new_msg', function(msg){
        console.log("收到消息："+msg);
    });
    // 后端推送来在线数据时
    socket.on('update_online_count', function(online_stat){
        console.log(online_stat);
    });
</script>
			
### 后端调用api向任意用户推送：
<?php
// 指明给谁推送，为空表示向所有在线用户推送
$to_uid = "";
// 推送的url地址，上线时改成自己的服务器地址
$push_api_url = "http://workerman.net:2121/";
$post_data = array(
   "type" => "publish",
   "content" => "这个是推送的测试数据",
   "to" => $to_uid, 
);
$ch = curl_init ();
curl_setopt ( $ch, CURLOPT_URL, $push_api_url );
curl_setopt ( $ch, CURLOPT_POST, 1 );
curl_setopt ( $ch, CURLOPT_HEADER, 0 );
curl_setopt ( $ch, CURLOPT_RETURNTRANSFER, 1 );
curl_setopt ( $ch, CURLOPT_POSTFIELDS, $post_data );
curl_setopt ($ch, CURLOPT_HTTPHEADER, array("Expect:"));
$return = curl_exec ( $ch );
curl_close ( $ch );
var_export($return);





