# JS --- 数组循环要用length - weixin_33985507的博客 - CSDN博客
2017年05月03日 14:03:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
```
socket.on("receive", function (data) {
    deviceone.print("返回的数据:"+data)  // 发送异常
    var array = parse.GetArray(data)
    
    for (var i = 0; i < array.length; i++) {
        
        var ip = parse.GetIpAddr(data, i)
        var date = parse.GetDate(data, i)
        var num = parse.GetNum(data, i);
        var connId = parse.GetConnId(data, i)
        deviceone.print("集中器信息: "+ ip + date + num + connId)
    }
})
```
