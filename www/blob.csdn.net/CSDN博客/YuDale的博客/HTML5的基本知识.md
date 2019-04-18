# HTML5的基本知识 - YuDale的博客 - CSDN博客
2017年02月24日 14:57:06[YuDale](https://me.csdn.net/YuDale)阅读数：152
HTML5中的一些标记和属性的使用示例
图片源自百度
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>标签作用介绍及效果展示</title>
</head>
<body>
<h1>标题标记:h1</h1>
<h2>标题标记:h2</h2>
<h3>标题标记:h3</h3>
<h4>标题标记:h4</h4>
<h5>标题标记:h5</h5>
<h7>标题标记:h6</h6>
<p>段落标记:p</p>
<p>水平线标记:hr</p>
<hr />
<p>水平线标记:hr 颜色，位置，宽度，粗细</p>
<hr color="red" align="right" width="70%" size="10" />
<p>换行<br />标记:br</p>
<p><strong>strong </strong><em>em </em><del>del</del> <ins>ins</ins></p>
<p>"绝对路径"，图片标签，图片不能加载时用于替代的说明，鼠标指向图片的悬浮说明,图片宽度，高度,垂直边距，水平边距</p>
<img src="D:\picture\00_head_portait.jpg" alt="这是个图片" title="这是只可爱的柴犬" width="256" height="256" border="10" vspace="100" hspace="100" id="one" />
<p>"相对路径"，图片标签，图片不能加载时用于替代的说明，鼠标指向图片的悬浮说明</p>
<img src="00_head_portait.jpg" alt="这是个图片" title="这是只可爱的柴犬" width="256" height="256"/><br />
<a href="http://www.baidu.com/" target="_self">百度，超链接，在原窗口打开</a>  target="_self"在原窗口打开<br />
<a href="http://www.baidu.com/" target="_bank">百度，超链接，在新建窗口打开</a> <br />
图片链接<br />
<a href="http://baidu.com/" target="_blank"><img src="D:\picture\01_head_portait.png" title="百度" width="256" height="256" />
<br /><br />
<br /><br />
<br /><br />
<br /><br />
<a href="#one">柴犬图片</a>
</body>
</html>
            
