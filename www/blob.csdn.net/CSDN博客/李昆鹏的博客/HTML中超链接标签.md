
# HTML中超链接标签 - 李昆鹏的博客 - CSDN博客


2018年04月23日 11:01:25[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：409


-------------------------------------------HTML中超链接标签-------------------------------------------------
**超链接标签: a**
属性：
Href提示我们连接的地址
Target打开方式 _blank 在一个新的页面打开
地址栏：D:\20130720\day29\code\test\www.baidu.com
注意：如果做超链接操作的时候，你要告诉href这个属性你采用的是什么类型的协议：
默认采用的是文件解析协议。
你想使用网址的时候，请自己加上http协议。
锚链接：
定义一个锚点，然后再和超链接结合使用
还是使用a标签定义锚点
定义锚点:
<a name=”锚点名”></a>
在360浏览器中定义 id
使用锚点：
<a href=”\#锚点名”>返回xxx</a>
**示例代码：**
<html>
<head>
<title>www.likunpeng.com</title>
</head>
<bady>
<h1>超链接的使用</h1>
<a  name="firsta" href="html_demo5.html">点击跳转到demo5</a><br/>
<a  href="www.baidu.com">点击跳转到百度</a><br/>
<a  href="http://www.baidu.com">点击跳转到百度</a><br/>
<a  href="html_demo4.html" target="_blank">点击跳转到demo4</a><br/>
<hr/>
<h1>超链接锚点的使用</h1>
<img  id="firstImg" src="3.jpg"title="包租婆怎么没水呢？"/>
<br><br><br><br><br><br><br><br><br><br><br><br><br><br>
<img  src="3.jpg" title="包租婆怎么没水呢？"><br/>
<a  href="\#firsta">返回第一个链接</a><br/>
<a  href="\#firstImg">返回第一个图片</a>
</bady>
</html>


