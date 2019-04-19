# jquery  发get post请求 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年08月17日 10:34:24[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：70
[https://www.cnblogs.com/summers/p/3225375.html](https://www.cnblogs.com/summers/p/3225375.html)
POST 方法不会缓存数据
$.get(URL,callback); 2个参数
 callback 参数是请求成功后所执行的函数名。
$("button").click(function(){ $.get("demo_test.asp",function(data,status){ alert("Data: " + data + "\nStatus: " + status); }); });
第二个参数是回调函数。第一个回调参数存有被请求页面的内容，第二个回调参数存有请求的状态。
状态值有：
- status - contains the status of the request ("success", "notmodified", "error", "timeout", or "parsererror")
[https://www.w3schools.com/jquery/ajax_get.asp](https://www.w3schools.com/jquery/ajax_get.asp)
$.post(URL,data,callback);
callback 参数是请求成功后所执行的函数名。
$("button").click(function(){ $.post("demo_test_post.asp", { name:"Donald Duck", city:"Duckburg" }, function(data,status){ alert("Data: " + data + "\nStatus: " + status); }); });
使用 AJAX 的 GET 请求来改变 div 元素的文本：
$("button").click(function(){ $.get("demo_ajax_load.txt", function(result){ $("div").html(result); }); });
html方法参见
[http://www.w3school.com.cn/jquery/jquery_dom_set.asp](http://www.w3school.com.cn/jquery/jquery_dom_set.asp)
这是一个简单的 GET 请求功能以取代复杂 $.ajax
请求成功时可调用回调函数。如果需要在出错时执行函数，请使用 $.ajax
[https://www.cnblogs.com/summers/p/3225375.html](https://www.cnblogs.com/summers/p/3225375.html)
