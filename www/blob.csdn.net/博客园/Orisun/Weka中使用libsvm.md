# Weka中使用libsvm - Orisun - 博客园







# [Weka中使用libsvm](https://www.cnblogs.com/zhangchaoyang/articles/2112049.html)





网上讲的还要下载libsvm.jar包，配置环境变量神马的，此处讲的方法不需要那么麻烦呵。

我的系统是Ubuntu，所以其他地方讲的更改配置文件RunWeka.ini云云在我这儿根本就没这个文件，Weka也不需要安装，解压后直接运行java -jar weka.jar就行了。运行jar包时官网上有这么一句提示：Note that Java needs to be installed on your system for this to work. Also note, that using `-jar` will override your current CLASSPATH variable and only use the `weka.jar`.

所以即使你下载了libsvm.jar并放到了$CLASSPATH中，在分类器中选择libsvm时还是会说libsvm不在$CLASSPATH中，因为这时weka使用的$CLASSPATH不是你系统中设置的$CLASSPATH，即使用java -cp path/to/libsvm.jar -jar weka.jar问题依旧。

首先各位要下载[Weka-3-7-4 developer edtion for linux](http://prdownloads.sourceforge.net/weka/weka-3-7-4.zip)，解压后运行java -jar weka.jar。打开Tools-->Package Manager

![](https://pic002.cnblogs.com/images/2011/103496/2011072020042231.png)

然后你在使用分类器的时候就可以选择使用libsvm算法了。












