# 深度学习基础--正则化与norm--局部响应归一化层(Local Response Normalization, LRN) - wydbyxr的博客 - CSDN博客
2018年11月20日 10:14:14[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：244
# 局部响应归一化层(Local Response Normalization, LRN)
  局部响应归一化层完成一种“临近抑制”操作，对局部输入区域进行归一化。
  该层实际上证明已经没啥用了，一般也不用了。
  参考资料：见邮件
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120101402465.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
## 公式与计算
  该层需要的参数包括：
  1）norm_region：选择对相邻通道间归一化还是通道内空间区域归一化，默认ACROSS_CHANNELS，即通道间归一化；
  2）local_size：两种表示。若是通道间归一化，表示求和的通道数；若是通道内归一化，表示求和区间的边长，默认值为5；
  3）alpha：缩放因子（详细见后面），默认值为1；
  4）beta：指数项（详细见后面）， 默认值为5；
  LRN按照标准公式，不包含任何查表、近似、展开等操作。计算量如下：
  设features map size为W,H，channel数为N，LRN某一个系数为Y，filter size为Fw，Fh，
  乘法：    2*W*H*N。
  加法：    Fw*Fh*W*H*N。
  幂运算：W*H*N次指数为Y的幂运算。
