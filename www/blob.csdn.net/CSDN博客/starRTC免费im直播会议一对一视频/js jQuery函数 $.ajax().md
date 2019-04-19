# js jQuery函数 $.ajax() - starRTC免费im直播会议一对一视频 - CSDN博客
2017年11月14日 12:21:27[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：119
**$.ajax()**
//$表示是jQuery
cache: 
要求为Boolean类型的参数，默认为true（当dataType为script时，默认为false），设置为false将不会从浏览器缓存中加载请求信息。
**type**
默认值: "GET" 请求方式 ("POST" 或 "GET")，
**data**: 
对象必须为key/value格式，例如{foo1:"bar1",foo2:"bar2"}
**dataType**: 
预期服务器返回的数据类型。如json：返回JSON数据。
**success**：要求为Function类型的参数，请求成功后调用的回调函数
**error**:
要求为Function类型的参数，请求失败时被调用的函数。该函数有3个参数，即XMLHttpRequest对象、错误信息、捕获的错误对象(可选)。ajax事件函数如下：
       function(XMLHttpRequest, textStatus, errorThrown){
          //通常情况下textStatus和errorThrown只有其中一个包含信息
       }
ajax() 方法通过 HTTP 请求加载远程数据。
简单易用的高层实现见 $.get, $.post 
参考
[http://blog.csdn.net/qingluoii/article/details/50330097](http://blog.csdn.net/qingluoii/article/details/50330097)
[http://www.w3school.com.cn/jquery/ajax_ajax.asp](http://www.w3school.com.cn/jquery/ajax_ajax.asp)
