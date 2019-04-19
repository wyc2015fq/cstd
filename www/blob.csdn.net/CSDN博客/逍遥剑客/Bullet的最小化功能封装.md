# Bullet的最小化功能封装 - 逍遥剑客 - CSDN博客
2010年01月03日 22:16:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2246标签：[图形																[引擎																[actor																[.net](https://so.csdn.net/so/search/s.do?q=.net&t=blog)](https://so.csdn.net/so/search/s.do?q=actor&t=blog)](https://so.csdn.net/so/search/s.do?q=引擎&t=blog)](https://so.csdn.net/so/search/s.do?q=图形&t=blog)
个人分类：[动画物理](https://blog.csdn.net/xoyojank/article/category/569711)
既然是"最小化", 那么就要明确最少需要哪些功能. 对于一般的物理应用来说, 只是简单地模拟碰撞, 柔体什么的都用不到. 在这样的前提下, bullet的工程可以精简到三个: libbulletcollision, libbulletdynamics, libbulletmath.
接下来, 需要明确一下基本的物理概念. 这个是跟用什么库(physx, havok, etc)是没有关系的. 在上面的前提下, 物理方面可以抽象成一个程序对象和三个物理对象:
- 程序对象: 就是对物理引擎的包装, 一般只有三个行为: 创建, 销毁, 更新.
- 物理对象: 
- 世界: 就是场景, 物体模拟发生的舞台. 基本功能有添加Actor(下面这俩东西), 设置重力, 射线拣取
- 静态物件: 不可移动的碰撞物件, 如房子, 树. 这个通常是由多个基本几何对象(box, capsule, sphere, cylinder...)组成.
- 动态刚体: 可以移动的或可被碰撞影响的物件, 如皮球, 人, 车等. 需要与图形表示(图形引擎中的模型)进行绑定的属性有位置, 朝向, 大小. 通常只需要从中获取变换矩阵并更新到图形引擎即可. 外部操作有受力, 动量(速度相关). 
另外, 物体之间还有一种约束关系(连接关系), 用于弹簧, 绳索, 转轴之类的模拟, 这里不考虑.
如果不需要多个场景, 可以把程序对象合并进物理对象, 只是为了方便使用, 设计的话太复杂了反而成了过度设计.
以Bullet的.net wapper为例, 它只在C-API的基础上抽象了三个对象:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20100103/BulletPhysics.jpg)
这只是最基本的功能, 不过在这基础上添加受力, 材质属性什么的就很容易了.
