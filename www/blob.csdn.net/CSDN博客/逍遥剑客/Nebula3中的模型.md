# Nebula3中的模型 - 逍遥剑客 - CSDN博客
2009年01月13日 23:42:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：1730
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
大体画了个结构图, UML不知道用得对不对, 反正我自己看懂就行了......
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090113/N3Model.JPG)
VisXXX可以看做是场景可见性裁剪的一部分, 渲染模块都是跟这个打交道的, 通过它可以把各个结点(node)排序以达到优化渲染目的.
Model是一个可以绘制的对象, 由一些树形结构的ModelNode组成. Model可以通过ModelServer创建, 这样能保持同一个Model只会载入内存一次. 要绘制Model的话, 一个ModelInstance是必不可少的. 通常一个游戏对象对应一个ModelInstance, 那些始终不变的数据(如网格)都保存在Model里. 
ModelNode是组成Model中树形结构的基本元素, 分别看看各个子类有何作用:
- TransformNode: 描述层级变换信息, 骨骼动画的最基本体现. 就是相对于父级的位置, 旋转, 缩放之类的数学信息.
- StateNode: 包含Shader的状态和纹理信息, 渲染用的
- ShapeNode: 说白了就是一Mesh, 网格, 三角形的集合
- SkinShapeNode: 描述了一个蒙皮的网格. 
- ParticleSystemNode: 粒子系统的结点, 代表一个动态网格.
- CharcterNode: 也是代表蒙皮网格, 具体没细看(模型这块下个版本的SDK估计改动很大, 所以只是大体先熟悉一下)
嗯, 基本上就是这样了, 还有个ManagedModel, 这个其实可以归到Resource子系统里去. 睡觉!
