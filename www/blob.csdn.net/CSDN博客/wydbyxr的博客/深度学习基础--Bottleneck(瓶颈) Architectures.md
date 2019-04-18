# 深度学习基础--Bottleneck(瓶颈) Architectures - wydbyxr的博客 - CSDN博客
2018年11月12日 11:13:47[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：577
个人分类：[深度学习基础](https://blog.csdn.net/wydbyxr/article/category/6829999)
# Bottleneck(瓶颈) Architectures
  ResNet的核心内容之一，即“Deeper Bottleneck Architectures”（以下简称DBA），论文里的原图是这样的（Bottleneck V1 ）：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181112111336907.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
## 作用
  换成bottleneck design以后，网络的参数减少了很多，训练也就相对容易一些。然后我们再看一下原作中的ResNet全貌。
