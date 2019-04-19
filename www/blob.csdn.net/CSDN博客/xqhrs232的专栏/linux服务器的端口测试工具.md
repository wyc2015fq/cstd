# linux服务器的端口测试工具 - xqhrs232的专栏 - CSDN博客
2016年03月23日 23:45:09[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：467
原文地址：[http://blog.sina.com.cn/s/blog_3eea4f680101fuxf.html](http://blog.sina.com.cn/s/blog_3eea4f680101fuxf.html)
相关文章
1、五款优秀的 Linux 端口扫描工具----[http://www.oschina.net/news/15639/5-linux-port-scanner](http://www.oschina.net/news/15639/5-linux-port-scanner)
有时服务器还没部署呢，运维会让你测试网络关系是否通了，这个时候你又没有时间腾出来取部署服务器，如何是好？
linux下有一个nc的命令行工具，可以绑定到端口，并且可以选择tcp和udp协议，这可以给我们很大的便利。
比如:
nc -l 3837 #监听3837端口
在需要测试连通性的机器上telnet上去即可。
nc会把telnet客户端端输出的东西原封不动的显示在接收端的屏幕上。
