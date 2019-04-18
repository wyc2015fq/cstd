# 深度学习基础--卷积--correlation layer(相关联层/互相关层) - wydbyxr的博客 - CSDN博客
2018年11月14日 10:05:50[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：355
# correlation layer(相关联层/互相关层)
  该层的主要作用是比较来自两张图片的feature map的关系，文章用了卷积的形式，还是比较巧妙的，具体方程如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114100525242.png)
  C代表correlation，f1和f2分别代表两个feature map， k代表要比较的区域，x1，x2分别代表两个feature map上的点。具体的计算流程还是看原文本，这里只要知道这个层的主要作用是计算两个层的关系就可以了。
以上来自https://blog.csdn.net/bea_tree/article/details/67049373
*以下是自己的一些分析，最好结合代码看*
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190312163806681.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190312163819202.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
