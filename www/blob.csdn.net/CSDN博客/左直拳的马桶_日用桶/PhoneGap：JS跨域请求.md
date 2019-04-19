# PhoneGap：JS跨域请求 - 左直拳的马桶_日用桶 - CSDN博客
2014年08月05日 19:09:21[左直拳](https://me.csdn.net/leftfist)阅读数：6473
个人分类：[JavaScript																[PhoneGap](https://blog.csdn.net/leftfist/article/category/2455151)](https://blog.csdn.net/leftfist/article/category/94500)
PhoneGap开发，理论上好处多多。但因为javascript是其中的主角，并且是直接存放于手机，跟服务器数据交互，就会有一个跨域访问的问题。
当然，这个问题肯定有解决方案，不然的话，这种利用PhoneGap等的混合APP模式就没有存在的价值。
网上的方案有很多，比如获取数据，可以采用Jsonp。
但提交数据呢？
首先可以在服务器端做一些配置，允许跨域请求。比如，对于ASP.NET开发的网站，可以修改web.config，加上：
web.config:
```
<system.webServer>
    <httpProtocol>
      <customHeaders>
        <add name="Access-Control-Allow-Methods" value="OPTIONS,POST,GET"/>
        <add name="Access-Control-Allow-Headers" value="x-requested-with,content-type"/>
        <add name="Access-Control-Allow-Origin" value="*" />
      </customHeaders>
    </httpProtocol>
  </system.webServer>
```
然后是客户端JS：
```java
$(function () {
	$("#divHello").html("Hello PhoneGap");
	forServer();
});  
var ashx = "http://10.69.25.132/test.ashx";
function forServer(){
	var xhr;
	xhr = new XMLHttpRequest();
	if (xhr){
		xhr.onerror = function(){alert("错误");};
		//xhr.timeout = 60000;//IE9加上这个属性会报错，不知道为什么，其他版本未测试
		xhr.ontimeout = function(){alert("超时");};
		xhr.onload = function(){
			var data = $.parseJSON(xhr.responseText);
			$("#divData").html(data.data);
		};
		xhr.open("post", ashx,true);
		xhr.setRequestHeader("Content-type","application/x-www-form-urlencoded");
		xhr.send("p1=guangzhou tianhe");
	}
	else{
		alert("Failed to create");
	}
}
```
服务器端test.ashx
```
public class test1 : IHttpHandler
    {
        public void ProcessRequest(HttpContext context)
        {
            context.Response.ContentType = "application/json";
            context.Response.Charset = "utf-8";
            string res = String.Format(@"{{""data"":""{0}""}}", 
                context.Request.Form["p1"]
                );
            context.Response.Write(res);
        }
        public bool IsReusable
        {
            get
            {
                return false;
            }
        }
    }
```
经验总结：
1、$.getJSON()的方式，Chrome下可以，IE死活不行，在IE浏览器下，按F12查看，根本没有连接指定的.ashx，直接拒绝。改用XMLHttpRequest，两种浏览器都能接受。放在安卓手机模拟器里也能正常运行
2、形如 http://localhost:8088/test.ashx 这种方式，在安卓手机模拟器里是连不上的，要改成IP或域名：http://192.168.10.1:8088/test.ashx
3、JS禁止跨域，不是为了保护被访问的服务器，而是为了保护使用浏览器的用户。只要想想木马、弹窗广告、居心叵测的信息收集，就会明白
4、$.getJSON()，IE不支持跨域访问，只能用XMLHttpRequest。有文章说IE8和IE9要使用 XDomainRequest  ，但其实貌似很是不行（再次鄙视微软，不知道为什么会搞出IE这种怪物，左摇右摆，到处是版本兼容问题，让人抓狂。）
```java
function getData(){
	$.getJSON(
		ashx
		,{"uid" : Date()}
		,function(data) {
			$("#divData").html(data.data);
		});
}
```

