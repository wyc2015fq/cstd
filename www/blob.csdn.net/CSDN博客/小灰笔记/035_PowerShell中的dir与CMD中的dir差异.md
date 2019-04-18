# 035_PowerShell中的dir与CMD中的dir差异 - 小灰笔记 - CSDN博客





2018年12月13日 23:10:29[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：300
个人分类：[PowerShell](https://blog.csdn.net/grey_csdn/article/category/6846632)









         经常在PowerShell中使用与CMD相同的命令获得成功，比如今天讨论的dir。PowerShell中的效果：

![](https://img-blog.csdnimg.cn/20181213231008683.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         CMD中的效果：

![](https://img-blog.csdnimg.cn/20181213231008715.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         不是百分百相同，但是很相似。新学到了一个操作，具体如下：

dir –Path *.sh –Recurse

输出如下：

![](https://img-blog.csdnimg.cn/20181213231008717.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         输出我磁盘相应目录中的bash脚本。最初以为这个是Windows CMD命令中dir的效果。但是，在CMD中使用的时候失败了。后来发现，其实这个就是一个功能绑定给我的假象。

![](https://img-blog.csdnimg.cn/20181213231008719.png)

         两个地方的命令其实还是有着本质的差异的，换句话说，其实这是dir与Get-ChildItem的差异。



