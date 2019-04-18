# 判断远程图片是否存在的JavaScript代码 - weixin_33985507的博客 - CSDN博客
2012年01月07日 22:44:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
**代码简介：**
用JavaScript判断远程图片是否存在，不存在话则替换指定的图片，指定图片可以是你网站的LOGO或其它图片，或显示提示信息。没想到用JS还能干这种活，以前都是用笨方法，也就是用ASP/PHP程序去判断，这下可好了，省了很多事。
**代码内容：**
```
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<title>判断远程图片是否存在的JavaScript代码 - www.webdm.cn</title>
</head>
<body>
<p>判断百度的LOGO是否存在：<br>
<IMG src="http://www.baidu.com/img/baidu_logo.gif"
onerror="javascript:this.src='http://www.webdm.cn/themes/pic/webdm_logo.gif'"></p>
<p>如果不存在，则替换成WebDm.cn 的 logo：<br><IMG src="http://www.baidu.com/img/baidu_logo11.gif"
onerror="javascript:this.src='http://www.webdm.cn/themes/pic/webdm_logo.gif'"></p>
</body>
</html>
<br />
<p><a href="http://www.webdm.cn">网页代码站</a> - 最专业的网页代码下载网站 - 致力为中国站长提供有质量的网页代码！
</p>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<title>判断远程图片是否存在的JavaScript代码 - www.webdm.cn</title>
</head>
<body>
<p>判断百度的LOGO是否存在：<br>
<IMG src="http://www.baidu.com/img/baidu_logo.gif" 
onerror="javascript:this.src='http://www.webdm.cn/themes/pic/webdm_logo.gif'"></p>
<p>如果不存在，则替换成WebDm.cn 的 logo：<br><IMG src="http://www.baidu.com/img/baidu_logo11.gif" 
onerror="javascript:this.src='http://www.webdm.cn/themes/pic/webdm_logo.gif'"></p>
</body>
</html>
<br />
<p><a href="http://www.webdm.cn">网页代码站</a> - 最专业的网页代码下载网站 - 致力为中国站长提供有质量的网页代码！
</p>
[复制代码](http://www.cnblogs.com/webdm/archive/2011/12/27/2303364.html)
```
