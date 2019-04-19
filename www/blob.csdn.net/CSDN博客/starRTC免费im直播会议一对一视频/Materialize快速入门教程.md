# Materialize快速入门教程 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年08月29日 15:47:28[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：286
[https://materializecss.com/](https://materializecss.com/)
[https://github.com/Dogfalo/materialize](https://github.com/Dogfalo/materialize)
[http://www.materializecss.cn/](http://www.materializecss.cn/)
1，下载
[http://materializecss.cn/bin/materialize-v0.97.8.zip](http://materializecss.cn/bin/materialize-v0.97.8.zip)
<!-- Compiled and minified CSS --> <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/materialize/0.97.8/css/materialize.min.css"> <!-- Compiled and minified JavaScript --> <script src="https://cdnjs.cloudflare.com/ajax/libs/materialize/0.97.8/js/materialize.min.js"></script>
一般将javascript文件放在页面body标签的末端以减少页面的加载时间。
必须在materialize.js之前引入jQuery
在线演示示例
[http://site.elesos.com/demo-web/starter-template/](http://site.elesos.com/demo-web/starter-template/)
颜色
每一种颜色的定义有一个基本的颜色类和一个可选的减轻或变暗的类。
背景颜色：
只需要增加颜色的名称和颜色深浅度到元素的class属性中。
<div class="card-panel teal lighten-2">This is a card panel with a teal lighten-2 class</div>
颜色值参考Color Palette：
[https://materializecss.com/color.html](https://materializecss.com/color.html)
网格:
用container类来包含我们的body内容,可以使你的页面内容居中。
 s1 表示 small-1，他的意思是 "小屏幕中的一列"。
s = small, m = medium, l = large
布局
如果有三个等宽度的div，我们定义每个div的宽度是4列，4+4+4 = 12，加起来刚好是12。
导航布局
对齐
动态阴影 <div class="card-panel hoverable"> Hoverable Card Panel</div>
更多参加示例页面：
[http://site.elesos.com/demo-web/starter-template/test.html](http://site.elesos.com/demo-web/starter-template/test.html)
