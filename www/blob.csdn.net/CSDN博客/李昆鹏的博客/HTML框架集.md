
# HTML框架集 - 李昆鹏的博客 - CSDN博客


2018年05月08日 16:14:45[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：781


-------------------------------------------HTML框架集--------------------------------------------------
**HTML框架集**
body和框架集标签不同时使用。
框架集标签：frameset
属性：
rows:行分割
cols:列分割
|--frame
属性：
src:指定要嵌入的页面
noresize="noresize"可以阻止border拖拽
name:用于指定展示的内容所在的frame
代码示例：
<htlml>
<head>
<title>www.likunpeng.com</title>
</head>
<body>我是头区域</body>
</html>
----------------------------------
<htlml>
<head>
<title>www.likunpeng.com</title>
</head>
<body>
<a href="2.jpg" target="_blank">菜单一</a><br/>
<a href="2.jpg" target="main">菜单二</a>
</body>
</html>
-------------------------------------------
<htlml>
<head>
<title>www.likunpeng.com</title>
</head>
<body>我是主区域</body>
</html>
--------------------------------------------------
<htlml>
<head>
<title>www.likunpeng.com</title>
</head>
<frameset rows="20%,*">
<frame src="top.html"></frame>
<frameset cols="20%,*">
<frame src="left.html" noresize="noresize"></frame>
<frame src="main.html" name="main"></frame>
</frameset>
</frameset>
</html>
----------------------------------------------------------
![](https://img-blog.csdn.net/20180508161420897?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



