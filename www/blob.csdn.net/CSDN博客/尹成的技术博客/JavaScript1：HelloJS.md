
# JavaScript1：HelloJS - 尹成的技术博客 - CSDN博客

2018年11月14日 16:09:07[尹成](https://me.csdn.net/yincheng01)阅读数：79


\#JavaScript 简介
JavaScript 是一种可以给网页增加交互性的编程语言。
熟悉 JavaScript 的好方法是“站在其他程序员肩膀上”，即Web 上有大量 JavaScript代码，复制一下并稍作修改，就可以供自己使用。
JavaScript 脚本语言，在 HTML 页面内容是包围在
\#Hello, World
老习惯，我们第一个 JavaScript 是向浏览器窗口输出“Hello, World!”，脚本代码（HelloWorld.html）如下：
`<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>Hello, World!</title>
<script>
  function SayHello(){
    document.getElementById("myMessage").innerHTML = "Hello, World!";  
  }
</script>
</head>
<body>
<script>
  document.write("<h1>我们的第一个 JavaScript 程序</h1>");
  document.write("<p id=\"myMessage\">哦， 试试下面的按钮...</p>");
</script>
<button type="button" onClick="SayHello()">尝试一下</button>
</body>
</html>`效果图如下：
![这里写图片描述](https://img-blog.csdn.net/20180912092759240?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjI4NjAyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180912092917700?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjI4NjAyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114143613461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

