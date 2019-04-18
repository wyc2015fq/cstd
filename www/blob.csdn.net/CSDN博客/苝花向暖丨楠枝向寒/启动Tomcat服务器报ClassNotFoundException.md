# 启动Tomcat服务器报ClassNotFoundException - 苝花向暖丨楠枝向寒 - CSDN博客

2018年02月02日 20:56:03[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：400


今天启动服务器的时候控制台一直报错![](https://img-blog.csdn.net/20180202205157661?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQwMjQ3MjYz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

上网各种搜，就没人遇到我这问题的。刚开始我以为是.class文件 没编译出来，后来我看有。然后 我又看web.xml是不是写错了。也没问题。 折腾了好几个小时。最后我怀疑是tomcat/lib 文件夹下的jar包 可能有一些损坏了。 然后 就去官网重新下载了tomcat，然后复制lib 里面的jar包，覆盖之前的就解决了问题。  具体的原因 我不太明白。 如果有懂的大神望点拨一二。       

如有遇到相同状况的人希望能够给予帮助，同时自己记录。

