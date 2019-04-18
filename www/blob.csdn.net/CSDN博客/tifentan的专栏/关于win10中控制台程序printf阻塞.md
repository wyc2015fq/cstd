# 关于win10中控制台程序printf阻塞 - tifentan的专栏 - CSDN博客

2017年03月27日 10:50:27[露蛇](https://me.csdn.net/tifentan)阅读数：751


今天调试个程序出现个诡异的事件，发现有几率阻塞在printf函数里。

后发现用鼠标点击控制台就会出现以上情况。

现猜想是点击控制台窗口导致程序认为你进入了“输入模式”，printf函数会被阻塞直到输入完成。

果然在控制台窗口按下enter键退出阻塞。

![奋斗](http://static.blog.csdn.net/xheditor/xheditor_emot/default/struggle.gif)！！！


