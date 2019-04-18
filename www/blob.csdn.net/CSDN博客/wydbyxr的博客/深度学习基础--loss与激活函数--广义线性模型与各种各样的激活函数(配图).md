# 深度学习基础--loss与激活函数--广义线性模型与各种各样的激活函数(配图) - wydbyxr的博客 - CSDN博客
2018年11月19日 10:15:45[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：44
# 广义线性模型是怎被应用在深度学习中?
  深度学习从统计学角度，可以看做递归的广义线性模型。广义线性模型相对于经典的线性模型(y=wx+b)，核心在于引入了连接函数g(.)，形式变为：y=g(wx+b)。
  深度学习时递归的广义线性模型，神经元的激活函数，即为广义线性模型的链接函数。逻辑回归（广义线性模型的一种）的Logistic函数即为神经元激活函数中的Sigmoid函数，**很多类似的方法在统计学和神经网络中的名称不一样**，容易引起初学者的困惑。
  下图是一个对照表：	
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181119101519188.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
  常见的激活函数：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181119101529670.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
  Leaky ReLU, popular for DCGAN networks [34] and for small datasets,
