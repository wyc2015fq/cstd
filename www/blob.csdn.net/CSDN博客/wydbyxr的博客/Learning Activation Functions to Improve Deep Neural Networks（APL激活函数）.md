# Learning Activation Functions to Improve Deep Neural Networks（APL激活函数） - wydbyxr的博客 - CSDN博客
2018年11月28日 10:20:03[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：28
个人分类：[深度学习论文阅读](https://blog.csdn.net/wydbyxr/article/category/7151097)
# Learning Activation Functions to Improve Deep Neural Networks
  提出了APL激活函数。
## 公式与示意图
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181128101918279.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181128101951488.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
  1）输入为N*H*W，令它们为maps-1，每张为map-1
  2）每张map-1的每个点会通过K种不同的分段函数（故APL会有2KHW个参数），则一张map-1会得到K张map-2。
  再将这K张中间map-2进行element-sum，得到1张map-3。
  3）经过2），我们会得到N张map-3，对这N张map-3卷积
