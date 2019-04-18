# 049_AUTOSAR学习笔记_FEE - 小灰笔记 - CSDN博客





2018年12月18日 23:45:22[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：243








![](https://img-blog.csdnimg.cn/20181218234455874.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         以上是一个FEE模块中，FEE模块头的状态枚举类型。

![](https://img-blog.csdnimg.cn/20181218234456600.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)  枚举类型：Fee群组头不状态。

         疑问：群组是个什么概念？

![](https://img-blog.csdnimg.cn/20181218234456886.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181218234456645.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         这几天，job这个词看了很多一直好奇是什么意思。看起来是操作类型的意思，大致也是读、写、擦、校验等。

![](https://img-blog.csdnimg.cn/20181218234456741.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         函数：取消Fee的操作。

         这里也有一个疑问：取消的时候会尝试恢复之前的状态吗？

![](https://img-blog.csdnimg.cn/20181218234456921.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         Fee擦除立即模块。

![](https://img-blog.csdnimg.cn/20181218234456920.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         强制交换到另一个群组，这么看起来这个驱动还是之前看过的那个文件系统的方式。这个功能是为了切换到下一个存储快。

![](https://img-blog.csdnimg.cn/20181218234456847.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         获取操作方式的结果。

![](https://img-blog.csdnimg.cn/20181218234456856.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         这个还是有点不是很明白什么含义。

![](https://img-blog.csdnimg.cn/20181218234456798.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         这个功能后面或许是一个依赖度很高的功能，最起码需要判断是否处于busy状态。

![](https://img-blog.csdnimg.cn/20181218234456736.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         这个就是一个简单的初始化函数。

![](https://img-blog.csdnimg.cn/20181218234456901.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         这个会不会跟Fls函数一样，需要周期性调度？

![](https://img-blog.csdnimg.cn/20181218234456894.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         Fee的读取。

![](https://img-blog.csdnimg.cn/20181218234456895.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         这个是Fee的写入。现在，读写的功能基本上都全了，可以尝试一下集成测试了。不过，目前还是缺少一个模块的分配的功能，我到底该用哪一块FLASH呢？

![](https://img-blog.csdnimg.cn/20181218234455843.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181218234455840.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         上面的配置中，用了两块FLASH？

![](https://img-blog.csdnimg.cn/2018121823445632.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         这个或许是最小的写入块大小？

![](https://img-blog.csdnimg.cn/20181218234456791.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         结合Fls驱动，可以看得出是用的哪几块Flash。





