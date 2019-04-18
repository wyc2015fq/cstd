# 机器学习-lesson2(线性回归、梯度下降、逻辑回归) - 苝花向暖丨楠枝向寒 - CSDN博客

2018年11月29日 23:07:16[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：40


# 线性回归

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181128113740120.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/2018112811390484.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==,size_16,color_FFFFFF,t_70)
**上图中的平面就是我们预测的结果，橙黄色的点就是真是值，可以发现预测结果和真是值之间是存在误差的，我们要做的就是将误差降到最低。误差实际上是符合正态分布的，所以我们就可以将其应用于概率密度函数如下图中第二个公式当中。**
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018112811344594.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==,size_16,color_FFFFFF,t_70)
**似然函数就是上图最后一行的函数的连成，可以将上图第三个函数理解为边缘概率，而似然函数理解为联合概率**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181128113504468.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20181128113518846.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==,size_16,color_FFFFFF,t_70)
**我们为了让误差最小只需要上面的化简结果的值最小，就能使预测结果真实性的概率最大**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181128113532975.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==,size_16,color_FFFFFF,t_70)
**上面的公式**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181128123937917.png)
**部分无法保证一定存在，如不存在是无法通过上面的公式进行求解的，所以上述方式在机器学习当中并不常用，下面引入另外一直方法，梯度下降法**

# 梯度下降法

我们将预算的误差想象成是一座山，山顶误差最大，山底误差最小，假设我们目前在某一个点，那么为了使误差最小，我们就需知道我改向哪个方向走才能下山的速度最快。计算出方向以后**走一小步**，注意步长一定要短，否则你可能到达另外一个山的半山腰处，从而认错山底。如下图所示
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181129220047425.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==,size_16,color_FFFFFF,t_70)
**梯度下降的策略**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181129221624253.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==,size_16,color_FFFFFF,t_70)

# 逻辑回归

逻辑回归是解决分类的问题
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181129221752256.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181129222831142.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181129223733799.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181129230512935.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==,size_16,color_FFFFFF,t_70)

