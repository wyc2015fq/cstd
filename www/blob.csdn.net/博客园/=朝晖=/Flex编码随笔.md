# Flex编码随笔 - =朝晖= - 博客园
# [Flex编码随笔](https://www.cnblogs.com/dhcn/p/7100198.html)
　　1、CSS定义最好放在application里面。
        2、数据源是数组数据时，最好把数组转换为ArrayCollection.
        3、List、CheckBox等控件的HttpService Params传参处理：
```
var params:URLVariables = new URLVariables();  
var ar:Array=new Array();  
ar.push("www");  
ar.push("eee");  
params.users=ar;
```
出来的查询字符串是:
&users=www&users=eee
        4、为了防止Flex的缓存清楚问题，可以再HttpService的回调函数最后调用HttpServiceObject.ClearResult()这是我从[http://shanky.org/zh-CN/tag/httpservice/](http://shanky.org/zh-CN/tag/httpservice/) 上得到的一个解决方案（下面的文章由于汉化原因，实际代码不可用）
> 
其中一名与会者问我的问题，问题的Flex HTTPService和IE浏览器缓存。 我答应提供详细的解决这个问题，所以在这里：
问题 ：重复HTTPService要求时，从软硬度（运行在一个实例的IE浏览器）许多倍最终没有外部的HTTP调用。 看来这些数据是从缓存。
原因是 ： Flash播放器搭载在浏览器上进行的HTTP调用。 IE浏览器缓存的响应的HTTP GET要求和发生的相同的URL传回的响应缓存。
解决方案 ：这个问题可以得到解决或者在服务器端或客户端。
服务器端的解决办法 ：设置HTTP头的响应，以避免返回响应缓存。
在HTML ： （在标题）
```
<META HTTP-EQUIV="Cache-Control" CONTENT="no-cache"> 
<META HTTP-EQUIV="expires" CONTENT="0">
```
在[PHP](http://lib.csdn.net/base/php) ： （在脚本）
 < /p>
标题（ “缓存控制：无缓存，必须重新” ） ; 
标题（ “截止日期：星期一， 1997年7月26日5时00分00秒格林尼治标准时间” ） ;
在JSP中： （前书面向输出流）
response.setHeader （ “缓存控制” ， “无缓存” ） ; 
response.setDateHeader （ “到期” ， 0 ） ;
客户端的解决办法 ： （ 1 ）的HTTP邮政电话-只的HTTP GET要求是从缓存或（ 2 ）确定的HTTP GET网址是不同的每一次。
（ 1 ）的HTTP后呼吁- 
设置方法= “ post ”和呼吁妥善处理
（ 2 ）附加了一个独特的参数的HTTP GET要求，这样的网址是不同的每一次。 一种独特的时间戳记是一个很好的选择。 
下面的代码示例，可以做的工作：
无功timeStampForNocache ：日期=新的日期（ ） ; 
params.noCacheControlVar = timeStampForNocache.getTime （ ） 。 toString （ ） ; 
我已命名的参数“ noCacheControlVar ” 。 您可以将它命名为任何您请。 这个名字并不重要。 要紧的是，时间戳使独特的HTTP GET的URL 。
        5、界面初始化时的多HttpService结果数据如果之间有条件关系，那可以用事件响应链来解决这个问题。因为FLex是单线程，同步标志的方法可能不可行。

