# a链接触发javascript函数导致innerHTML里的图片无法加载 - 左直拳的马桶_日用桶 - CSDN博客
2011年05月09日 00:53:00[左直拳](https://me.csdn.net/leftfist)阅读数：2736标签：[javascript																[脚本																[ie																[div																[google](https://so.csdn.net/so/search/s.do?q=google&t=blog)](https://so.csdn.net/so/search/s.do?q=div&t=blog)](https://so.csdn.net/so/search/s.do?q=ie&t=blog)](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)
个人分类：[JavaScript](https://blog.csdn.net/leftfist/article/category/94500)
偶尔发现，IE6下，用javascript控制的div的innerHTML，如果里面有图片，居然无法显示！
难道用脚本来给innerHTML赋值就不行的？
其实不是。因为有些地方又可以。
google了一下，才知道，原来并不是用脚本给innerHTML赋值的问题，而是因为用了a链接，靠点击这个a链接来触发给innerHTML赋值的脚本，就导致了innerHTML里面包含的图片无法加载！
<a href="javascript:;" onclick="LoadIt()
">
打开一个折叠项</a>
<div id="divContent"></div>
……
<script type="text/javascript">
function LoadIt()
{
//加载了之后，图片只有一个大大的叉
document.getElementById("divContent").innerHTML = "<img src='hi.jpg' />";
}
</script>
据说这是IE6，IE7的BUG。原因是你点了这个a链接，靠它来激发函数LoadIt()，IE会以为你已经跳走了，就将所有的请求都断掉，于是图片无法加载！或者只出来一小部分！
解决办法是，不要用a链接来触发，改用div或者span。
