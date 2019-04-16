# 配置class PATH - ggabcda的博客 - CSDN博客





2017年09月13日 18:46:05[ggabcda](https://me.csdn.net/ggabcda)阅读数：141








声明：本栏目所使用的素材都是凯哥学堂VIP学员所写，学员有权匿名，对文章有最终解释权；凯哥学堂旨在促进VIP学员互相学习的基础上公开笔记。

PATH->是用来配置DOS中的快捷命令，里面存储很多路径，从前置方式去搜索

PATHEXT->后缀配置，搜索的时候配合你名字进行搜索

DOS中的临时环境变量（DOS关闭就失效）：

1.SET环境变量名称=环境变量内容

2.临时的环境变量配置会把之前的环境变量内容全部覆盖掉，所以以前的PATH内容就丢失了

3.SET PATH=C:\program Files\Java\jdk1.8.0_144\bin;%path%

这句话配置了一个临时环境变量path，后面是引用之前path里面配置内容，组合在一起了

![image](http://os78f4hny.bkt.clouddn.com/201709120001.jpg?imageMogr2/blur/1x0/quality/75%7Cwatermark/1/image/aHR0cDovL29zNzhmNGhueS5ia3QuY2xvdWRkbi5jb20vd2F0ZXJtYXJrLnBuZw==/dissolve/50/gravity/SouthEast/dx/10/dy/10%7Cimageslim)

错误：

在DOS里出现此命令不是内部过着外部程序，那就是你的PATH 没有配置好

![image](http://os78f4hny.bkt.clouddn.com/201709120002.jpg?imageMogr2/blur/1x0/quality/75%7Cwatermark/1/image/aHR0cDovL29zNzhmNGhueS5ia3QuY2xvdWRkbi5jb20vd2F0ZXJtYXJrLnBuZw==/dissolve/50/gravity/SouthEast/dx/10/dy/10%7Cimageslim)

DOS（磁盘管理系统）：

1.管理磁盘文件

2.执行相关的程序

![image](http://os78f4hny.bkt.clouddn.com/201709120003.jpg?imageMogr2/blur/1x0/quality/75%7Cwatermark/1/image/aHR0cDovL29zNzhmNGhueS5ia3QuY2xvdWRkbi5jb20vd2F0ZXJtYXJrLnBuZw==/dissolve/50/gravity/SouthEast/dx/10/dy/10%7Cimageslim)

找不着

![image](http://os78f4hny.bkt.clouddn.com/201709120004.jpg?imageMogr2/blur/1x0/quality/75%7Cwatermark/1/image/aHR0cDovL29zNzhmNGhueS5ia3QuY2xvdWRkbi5jb20vd2F0ZXJtYXJrLnBuZw==/dissolve/50/gravity/SouthEast/dx/10/dy/10%7Cimageslim)

1.你的路径下是不是没有这个Hello.alss

2.你的lassPATH是不是没有配置

3.大小写问题

什么是class版本问题

1.你是用高版本的jdk编译的Hello.java

2.然后在低版本中运行了Hello.class

![image](http://os78f4hny.bkt.clouddn.com/201709120005.jpg?imageMogr2/blur/1x0/quality/75%7Cwatermark/1/image/aHR0cDovL29zNzhmNGhueS5ia3QuY2xvdWRkbi5jb20vd2F0ZXJtYXJrLnBuZw==/dissolve/50/gravity/SouthEast/dx/10/dy/10%7Cimageslim)

Javaclass的兼容问题：

1.编译的class文件可以向上兼容

2.编译的class文件不可以向下兼容



