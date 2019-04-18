# centos图形界面和文本界面登陆切换设置 - 专注于数据挖掘算法研究和应用 - CSDN博客





2012年04月08日 08:57:20[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：7409








vim /etc/inittab  

1）打开后最后一行将5改成3，实现centos文本模式启动，默认ssh连接登录；

2）同样将3改回5，实现centos图形模式启动；



