# dll中使用到容器的全局变量在mfc中报内存泄漏 - tifentan的专栏 - CSDN博客





2017年11月27日 10:28:50[露蛇](https://me.csdn.net/tifentan)阅读数：155








这几天发现一个蛋疼的问题，在vs2015上，dll中使用到容器的全局变量在mfc中报内存泄漏。 
![这里写图片描述](https://img-blog.csdn.net/20171127095131629?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlmZW50YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

  这是怎么出现的呢，我加入了opengl的依赖后就出现了。是不是很神奇？而且只针对那些使用到容器的全局变量。
例如 

```
char tmp[111]; //不会报泄漏。
  std::string sd = "why is that happened"; //报泄漏
```

那到底是什么原因呢？ 

  我猜是由于opengl库的加入扰乱了原来进程卸载时的模块释放顺序（这不代表出错），在mfc主程序退出时检测内存泄漏的时刻，dll里面的容器全局变量还没析构导致误报，可能在mfc主程序检测完才会析构。另外，opengl能有这么大“能耐”可能是因为他是系统自带库且全局容器变量特殊处理有关。 

  基于这猜测网上找一下别人会不会出现这问题。找到一些类似的。 
[http://blog.csdn.net/clever101/article/details/7907417](http://blog.csdn.net/clever101/article/details/7907417)
[http://blog.csdn.net/changbaolong/article/details/7935192](http://blog.csdn.net/changbaolong/article/details/7935192)

  不是我的问题就好。 

  对了在vs2015上检测内存泄漏有自带的工具，很方便，下次专门写个文章介绍。




