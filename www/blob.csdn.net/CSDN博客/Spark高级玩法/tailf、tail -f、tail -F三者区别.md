# tailf、tail -f、tail -F三者区别 - Spark高级玩法 - CSDN博客
2018年01月30日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：215
数据采集，浪尖公司一直是自己公司写的agent和插件，今天新增业务要快速上线，就想试试flume。结果是用flume，采用tail -f 监控文件的方式，然后发现文件110MB滚动一次，几乎是三四分钟就会重命名并重新生成监控的文件名，所以就出现了bug。经群友（欢迎大家加群，在公众号菜单栏里查找）指正，应该修改为tail -F才能避免这个问题，然后查资料得出如下结论：
tail -f      等同于--follow=descriptor，根据文件描述符进行追踪，当文件改名或被删除，追踪停止
tail -F     等同于--follow=name  --retry，根据文件名进行追踪，并保持重试，即该文件被删除或改名后，如果再次创建相同的文件名，会继续追踪
tailf        等同于tail -f -n 10（貌似tail -f或-F默认也是打印最后10行，然后追踪文件），与tail -f不同的是，如果文件不增长，它不会去访问磁盘文件，所以tailf特别适合那些便携机上跟踪日志文件，因为它减少了磁盘访问，可以省电。
**推荐阅读：**
[重磅：Flume1-7结合kafka讲解](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484218&idx=1&sn=fe25af996f713b12359bd6e18cef5f31&chksm=9f38e012a84f69044ff9285b9f70e5e8aa6d6458faa306ac59645e307a8791b7acac77d156dc&scene=21#wechat_redirect)
![640.png?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png?wxfrom=5&wx_lazy=1)
**关于Spark学习技巧**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg?wxfrom=5&wx_lazy=1)
**更多文章，敬请期待**
