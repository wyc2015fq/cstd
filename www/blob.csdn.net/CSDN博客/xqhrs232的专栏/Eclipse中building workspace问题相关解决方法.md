# Eclipse中building workspace问题相关解决方法 - xqhrs232的专栏 - CSDN博客
2011年06月20日 10:47:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：10492
**1.Eclipse中一直building workspace...的问题----修改eclipse.ini配置文件**
原文地址::[http://blog.csdn.net/daryl715/archive/2007/12/18/1946897.aspx](http://blog.csdn.net/daryl715/archive/2007/12/18/1946897.aspx)
导读： 
　　版本号： Eclipse SDK Version: 3.2.0 
　　现象：启动或者import新的project时，Eclipse会自动进行building workspace...这个操作，然后一直持续这个状态不再响应其他事件操作。有时也会在10几分钟后完成这个操作，但是费时严重。
　　解决方法：查了很多资料，最后在一个BBS上查到可能是启动参数设置的问题 
　　([http://www.myeclipseide.com/PNphpBB2+file-viewtopic-t-8253.html](http://www.myeclipseide.com/PNphpBB2+file-viewtopic-t-8253.html))
　　在Eclipse启动时加入参数： 
　　-vmargs -Xmx512M 
　　重启，building workspace...在短时间内结束，一切正常，具体原因可参考上面网址中的回答，可以给人很多提示，呵呵，看来还是多查资料有好处。写出来希望有相同现象的同仁可以参考解决。
　　Trackback: [http://tb.blog.csdn.net/TrackBack.aspx?PostId=1506871](http://tb.blog.csdn.net/TrackBack.aspx?PostId=1506871)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/daryl715/archive/2007/12/18/1946897.aspx](http://blog.csdn.net/daryl715/archive/2007/12/18/1946897.aspx)
**2.Eclipse Building Workspace 慢的要死**
原文地址::[http://leepiao.blog.163.com/blog/static/48503130201032645637274/](http://leepiao.blog.163.com/blog/static/48503130201032645637274/)
Eclipse  Building Workspace 速度慢，一直等待的问题。
试过了取消各种文件的validation，也试了在 Eclipse.ini文件中增加最大内存使用数量-vmargs-Xms40m-Xmx512m，都不管用。
后来发现是subclipse插件的问题，在 Help->Software Updates->Manage Configration中unenable掉subclipse插件，一切OK！

**3.Eclipse中一直building workspace的问题**
原文地址::[http://pkw.iteye.com/blog/1028580](http://pkw.iteye.com/blog/1028580)
最近用Eclipse开发oPhone的一个项目,每次打开Eclipse的时候,总是在build workspace,而且还老是build不完 
因为没有build完,就无法保存,运行和调试,关闭程序则会没有反应,只能结束进程,这让我相当的郁闷. 
后来找到这个文章:http://www.eclipsezone.com/eclipse/forums/t61566.html 
明白这样的诡异的原因是什么 
需要给Eclipse清理一下 
我在eclipse.ini式中添加了一个命令 -clean 
问题就解决了. 
**4.Eclipse 的 Building workspace 讨论网帖---取消build automatically**
原文地址::[http://hi.baidu.com/xiaoyun0708/blog/item/a97ccaf822c49e0ad9f9fda2.html](http://hi.baidu.com/xiaoyun0708/blog/item/a97ccaf822c49e0ad9f9fda2.html)
