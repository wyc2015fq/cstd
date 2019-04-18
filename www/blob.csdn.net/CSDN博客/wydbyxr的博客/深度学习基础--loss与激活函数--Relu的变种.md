# 深度学习基础--loss与激活函数--Relu的变种 - wydbyxr的博客 - CSDN博客
2018年11月19日 10:06:11[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：104
# Relu的变种
## softplus/softrelu
  softplus 是对 ReLU 的平滑逼近的解析函数形式。
  softplus的公式：
  f(x)=ln(1+e^x)
## Relu与PRelu
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181119100558428.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
  ai是增加的参数，ai=0；为ReLU，若ai取很小的固定值，则为leaky ReLU，这个激活函数可以修正ReLU频繁出现神经元die的情形，至于哪种更好没有定论。
