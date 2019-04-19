# nowrap要与回车换行符结合才有意义 - 左直拳的马桶_日用桶 - CSDN博客
2010年09月21日 16:38:00[左直拳](https://me.csdn.net/leftfist)阅读数：1410标签：[服务器																[table](https://so.csdn.net/so/search/s.do?q=table&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)
个人分类：[.NET																[Web](https://blog.csdn.net/leftfist/article/category/674421)](https://blog.csdn.net/leftfist/article/category/94497)
像一堆标签这种东西在页面上如何显示？不做特殊处理，很有可能有些标签会从中断开，换行，比如说，两个字的标签，会出现头一个字在上一行末尾，而另一个字出现在下一行的开头。
<span>君住长江头</span>
对付这种情况，应该用 style="white-space:nowrap;"
<span style="white-space:nowrap;"
>君住长江头</span><span style="white-space:nowrap;">我住长江尾</span>
但是这样问题又来了。在服务器端输出一大堆标签：
StringBuilder sb = new StringBuilder("");
foreach(...)
{
   sb.Append("<span style=/"white-space:nowrap;/">标签</span>");
}
label1.Text = sb.ToString();
结果在页面上，这些标签全部在一行，将页面撑爆鸟。即使用万能的<table>来框住也没有效果。
这个时候要引入回车换行符
StringBuilder sb = new StringBuilder("");
foreach(...)
{
   sb.Append("<span style=/"white-space:nowrap;/">标签</span>/r/n
");
}
label1.Text = sb.ToString();
这样每个标签都不会换行，但又不至于所有的标签都挤在一行。
