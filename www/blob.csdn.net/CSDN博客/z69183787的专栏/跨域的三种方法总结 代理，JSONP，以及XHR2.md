# 跨域的三种方法总结:代理，JSONP，以及XHR2 - z69183787的专栏 - CSDN博客
2015年07月31日 20:28:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2654
Javascript出于安全方面的考虑，不允许跨域调用其他页面的对象。
**什么是跨域呢？**
> 
简单地理解就是因为javascript同源策略的限制，a.com域名下的js无法操作b.com或是c.a.com域名下的对象。
**跨域处理，三种方法：**
1、处理跨域方法一 **代理**
通过在同域名下的web服务器端创建一个代理： 
北京服务器(域名:www.beijing.com) 
上海服务器(域名：www.shanghai.com) 
比如在北京的web服务器的后台(www.beijing.com/proxy-shanghaiservice.php)来调用上海服务器(www.shanghai.com/services.php)的服务，然后再把访问结果返回给前端，这样前端调用北京同域名的服务就和调用上海的服务效果相同了。
**2、处理跨域方式二——JSONP(只支持GET请求)：**
JSONP可用于解决主流浏览器的跨域数据访问的问题。 
在www.aaa.com页面中： 
<script> 
function jsonp(json){ 
alert(json["name"]); 
} 
</script> 
<script src="http;//www.bbb.com/jsonp.js"></script> 
在www.bbb.com页面中： 
jsonp({'name':'xx','age':24})
**3、处理跨域的方法三——XHR2：**
1、HTML5提供的XMLHttpRequest Level2已经实现了跨域访问以及其他的一些新功能 
2.IE10以下的版本都不支持 
3.在服务器端 
header('Access-Control-Allow-Origin:*'); 
header('Access-Control-Allow-Methods:POST,GET'); 
