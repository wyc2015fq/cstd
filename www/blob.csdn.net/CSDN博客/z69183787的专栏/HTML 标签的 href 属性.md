# HTML  标签的 href 属性 - z69183787的专栏 - CSDN博客
2012年10月29日 16:14:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1737
## 定义和用法
href 属性规定页面中所有相对链接的基准 URL。
```
<html>
<head>
<base href="http://www.w3school.com.cn/i/" />
</head>
<body>
<img src="eg_smile.gif" /><br />
<p>请注意，我们已经为图像规定了一个相对地址。由于我们已经在 head 部分规定了一个基准 URL，浏览器将在如下地址寻找图片：</p>
<p>"http://www.w3school.com.cn/i/eg_smile.gif"</p>
</body>
</html>
```
![](https://img-my.csdn.net/uploads/201210/29/1351498727_5600.jpg)
