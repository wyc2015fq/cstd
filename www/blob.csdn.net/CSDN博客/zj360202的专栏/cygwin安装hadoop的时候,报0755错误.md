# cygwin安装hadoop的时候,报0755错误 - zj360202的专栏 - CSDN博客





2012年12月10日 17:01:19[zj360202](https://me.csdn.net/zj360202)阅读数：414标签：[cygwin																[Cygwin																[hadoop																[Hadoop](https://so.csdn.net/so/search/s.do?q=Hadoop&t=blog)
个人分类：[cygwin																[Hadoop](https://blog.csdn.net/zj360202/article/category/1298102)](https://blog.csdn.net/zj360202/article/category/1301143)





在hadoop namenode -format成功格式化后，start-all.sh的时候，jobtracker和tasktracker没有正常启动，日志文件中报d:\hadoop\hadoop-1.0.1\logs\history\done to 0755，权限不够的错误，直接到这个目录，将这个目录的权限，右击，点掉"只读"这个选项，stop-all.sh一下hadoop，再次启动(start-all.sh)就好了

![](https://img-my.csdn.net/uploads/201212/10/1355130101_6388.jpg)

![](https://img-my.csdn.net/uploads/201212/10/1355130364_9794.jpg)

完整的安装，建议查看下面的文章

[http://www.zihou.me/html/2010/02/19/1525.html](http://www.zihou.me/html/2010/02/19/1525.html)](https://so.csdn.net/so/search/s.do?q=hadoop&t=blog)](https://so.csdn.net/so/search/s.do?q=Cygwin&t=blog)](https://so.csdn.net/so/search/s.do?q=cygwin&t=blog)




