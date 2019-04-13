
# jquery、javascript实现(get、post两种方式)跨域解决方法 - 高科的专栏 - CSDN博客

2018年05月25日 18:18:21[高科](https://me.csdn.net/pbymw8iwm)阅读数：544


﻿﻿
# [jquery、javascript实现(get、post两种方式)跨域解决方法](https://www.cnblogs.com/yxhblogs/p/6723001.html)
一、实现get方式跨域请求数据
浏览器端
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<script>$(document).ready(function(){
    $.ajax({
        url:"http://www.xxx.cn/index.php",
        type:"get",
        dataType:"jsonp",//指定服务器返回的数据类型success:function(data) {
            console.log(data);
        }
    });
});</script>![复制代码](https://common.cnblogs.com/images/copycode.gif)
服务器端
|1
|2
|3
|4
|5
|6
|7
|8
|<?php
|$data|= [
|'info'|=>|'跨域请求成功'|,
|'status'|=> 1
|];
|$callback|=|$_GET|[|'callback'|];|//callback参数是发起请求是jquery的回调参数。
|echo|$callback|.|'('|.json_encode(|$data|).|')'|;|//所有返回数据时，需要按照此方式。
|?>
|


二、实现post方式跨域请求
浏览器端
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|<script>
|$(document).ready(|function|(){
|$.ajax({
|url:|"http://www.xxx.cn/index.php"|,
|type:|"post"|,
|dataType:|"json"|,|//指定服务器返回的数据类型
|success:|function|(data) {
|console.log(data);
|}
|});
|});
|</script>
|


**服务器端**
Access-Control-Allow-Origin:*表示允许任何域名跨域访问
如果需要指定某域名才允许跨域访问，只需把Access-Control-Allow-Origin:*改为Access-Control-Allow-Origin:允许的域名
例如：header('Access-Control-Allow-Origin:http://www.xxx.com');
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|<?php
|header(|'Access-Control-Allow-Origin: *'|);|//允许所有来源访问
|header(|'Access-Control-Allow-Methods: POST'|);|//响应类型
|$data|= [
|'info'|=>|'post方式,跨域请求成功'|,
|'status'|=> 1
|];
|echo|json_encode(|$data|);
|?>
|


**某些header请求头**
// 指定允许其他域名访问
header('Access-Control-Allow-Origin:*');
// 响应类型
header('Access-Control-Allow-Methods:POST');
// 响应头设置
header('Access-Control-Allow-Headers:x-requested-with,content-type');
```python
//设置内容类型为json
```
header('content-type:application:json;charset=utf8');


