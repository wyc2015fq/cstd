
# Linear classifiers do not share parameters among features and classes - lpty的博客 - CSDN博客

2018年11月01日 10:23:25[lpty](https://me.csdn.net/sinat_33741547)阅读数：65标签：[fasttext																](https://so.csdn.net/so/search/s.do?q=fasttext&t=blog)个人分类：[论文阅读																](https://blog.csdn.net/sinat_33741547/article/category/7727913)



## 一、概述
标题来自Bag of Tricks for Efficient Text Classification，就是fasttext这篇文章
## 二、简析
### 1、解析
下述截图来自：[https://datascience.stackexchange.com/questions/30605/what-is-meant-by-sharing-of-parameters-between-features-and-classes](https://datascience.stackexchange.com/questions/30605/what-is-meant-by-sharing-of-parameters-between-features-and-classes)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181101101906740.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181101101916461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
上面对标题解释的比较清楚了
[
](https://img-blog.csdnimg.cn/20181101101916461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
### 2、后续
[
](https://img-blog.csdnimg.cn/20181101101916461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)在论文面还接了几句话
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181101102110782.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
主要说了解决方法，结论应该就对于高维稀疏空间的泛化问题，通用解决方式是将高维映射到低维，多层神经网络也是同一个道理。
[
](https://img-blog.csdnimg.cn/20181101102110782.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)有时候论文看着看着就蒙了，就算看过了，还是得认真仔细的看啊。
[
            ](https://img-blog.csdnimg.cn/20181101102110782.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)

[
  ](https://img-blog.csdnimg.cn/20181101101916461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)