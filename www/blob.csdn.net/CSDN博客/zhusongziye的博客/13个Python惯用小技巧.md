# 13个Python惯用小技巧 - zhusongziye的博客 - CSDN博客





2018年11月11日 16:09:14[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：82








Python的小技巧很多，入门容易精通难！**在进阶的路上，有没有什么好的技巧和好的方法，就是不断总结，不断记笔记**！尤其是好的用法，就像写作文一样，好的名言警句要多背诵一些，写作的时候，肚子里的墨水多了才能才思泉涌，写出更多的好代码。



1、**遍历一个序列**

![](https://img-blog.csdnimg.cn/2018111115562457.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

**评:Python中的序列是使用频率非常高的，很多传统语言过来的，习惯用下标遍历，Python中序列是可迭代的，直接for即可！**



2、**遍历倒序**

![](https://img-blog.csdnimg.cn/20181111155704511.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**评：用range的负数来遍历，不如直接反转遍历**



3、**遍历2个collection**

![](https://img-blog.csdnimg.cn/20181111155728797.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**评: zip真的不错,尤其是处理2个序列非常简洁，在Python2.x还需要用itertool.izip来返回迭代器，现在Py3可以直接使用zip返回迭代器。**



4、**遍历排序的序列**

![](https://img-blog.csdnimg.cn/20181111155749288.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**评: sorted这个函数非常重要，尤其是入门的菜鸟一定要记住这个函数，很多妙的用法都跟它有关。**



5、**自定义排序**

![](https://img-blog.csdnimg.cn/20181111155835393.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**评：排序是一个时时刻刻都遇到的文章，有列表排序，字典排序，排序里面的key用法很重要，甚至自定义一些函数去处理**



6、**遍历文件遇到指定字符退出**

![](https://img-blog.csdnimg.cn/20181111155900587.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**评：iter是一个内置函数用来生产迭代器，partial的不断的读入文件中32字节，****注意iter引入第二个参数，表示当读入的内容是''的时候，会触发生成器stop！**



7、**函数遍历多出口问题**

![](https://img-blog.csdnimg.cn/20181111155921711.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**评：for else这种用法跟传统的很不一样，一开始有点不习惯，熟悉了之后发现在搜索的时候经常会这样写。**



8、**字典的循环**

![](https://img-blog.csdnimg.cn/20181111160124178.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**评：字典的循环，这个是基本功，必须要掌握。**



9、**字典的统计**

![](https://img-blog.csdnimg.cn/20181111160739990.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**评：巧妙的利用字典的get用法，取不到value的时候用缺省值0**



10、**字典的统计**

![](https://img-blog.csdnimg.cn/20181111160759170.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**评：有点类似合并同类项，这样的场景一般用defaultdict处理会简洁很多**



11、**展开序列**

![](https://img-blog.csdnimg.cn/20181111160821603.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**评：要学会利用tuple展开，会更快更好，而且很多源码都是这样处理的。**



12、**交换变量**

![](https://img-blog.csdnimg.cn/20181111160837518.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**评:让我想起来的冒泡排序，交换变量，Python可以直接在一行中交换！**



13、**更新序列**

![](https://img-blog.csdnimg.cn/20181111160854685.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**评：利用collections模块里面的双向队列，会更简洁而且看的更舒服。**



上面的内容是我在逛Github的时候看到了，跟大家分享一下。限于篇幅没有展开，具体的链接：https://github.com/JeffPaine 。这些技巧需要细细咀嚼，慢慢消化，然后记下来，比如记住了collections模块，zip，sorted 等等，最好多敲几遍加深影响！



