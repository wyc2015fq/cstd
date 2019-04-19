# 在ASPX页面中输出XML - 左直拳的马桶_日用桶 - CSDN博客
2006年11月07日 17:44:00[左直拳](https://me.csdn.net/leftfist)阅读数：2059
在ASPX页面中输出XML
左直拳
因为工作关系接触了一下RSS。了解得不多，按我的理解，Rss feed其实就是一个XML文件。
不过这个XML文件往往不是从一开始就写好的静态文件摆在那里，而是根据条件有系统输出。比如，我新浪的博客的Rss feed其实就是：[http://blog.sina.com.cn/myblog/index_rss.php?uid=1174724855](http://blog.sina.com.cn/myblog/index_rss.php?uid=1174724855)
类似地，怎么在ASPX里输出XML代码呢？
可以这样写：
Response.Clear();
Response.ContentType = "text/xml";
Response.Write(xml);//XML是预先拼写好的XML代码
Response.End();
虽然很简单，但我却找了很久。幸甚至哉，博以咏志。
