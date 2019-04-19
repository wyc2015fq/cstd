# 在Nebula3中加载自定义模型的思路 - 逍遥剑客 - CSDN博客
2009年07月08日 22:18:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2397
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
Nebula3中目前还是没有地形…
不过冒似国外的引擎除了CryEngine外对于地形都不重视…..
之前看了下WOW的地形组织方式, 一直在想怎么把它加进来
模模糊糊地感觉到, 它应该是把Model当成了最基本的渲染单位
地形是不是也需要包装成模型呢? 也就是说, 地形对于引擎来说是个上层概念, 不应该放到Graphics内部去管理. 嗯, 虽说地形也是一种特殊的模型, 但它的管理方式相对来说太过于特殊了, 不知道还能不能跟模型走一条管线. 
先看看植被是怎么组织的:
![](http://images.blogcn.com//2009/7/8/9/xoyojank,m_20090708212028593.jpg)
(这里不讨论其原理, 因为这个系统是依赖Nebula Maya Toolkit的)(另: 这设计简直是中央集权….几乎每个类都要跟renderer打交道-_-)
![](http://images.blogcn.com//2009/7/8/9/xoyojank,20090708212028383.jpg)
嗯, 这里的渲染单位是GrassPatch, 它包装了一个InternalModelEntity, 然后扔给InternalStage就完成它的使命了. 下面主要看看这个graphicsEntity是怎么生出来的:
InternalModelEntity是场景管理中的基本图形对象(同级的还有摄像机和灯光), 这里面包含了一个ManagedModel.
ManagedModel是对于实际资源(Model)的管理包装, 见[资源子系统](http://blog.csdn.net/xoyojank/archive/2009/02/02/3179377.aspx). 根据资源的使用反馈来动态调整资源的细节等级. 资源的管理/加载都是在这一模块中进行的
Model就代表实际的模型了, 它由一系列层次结构的ModelNode组成. 在这里只有ShapeNode, 即静态图形. 通常情况下这就已经可以用来表示一个三维世界了. 至于粒子/动画之类的, 那是另外的ModelNode派生子类. 
ShapeNode中包装的是ManagedMesh, 不过这个就不用手动去构造了, N3提供一个MemoryMeshLoader, 直接从VB/IB进行构造. 由于ShapeNode是继承于StateNode, 那它同时也包含了渲染所需要的材质(shader, shader parameters, textures)
VertexBuffer和IndexBuffer的构造就简单多了, 之前写的几个小例子都是直接从内存加载的.
那么, 反过就是InternalModelEntity的自定义构造流程:
1.把顶点数据加载到内存, 利用MemoryVertexBufferLoader创建出VertexBuffer. IndexBuffer同理, 还有它俩相关的PrimitiveGroup…(看来都比倾向于一个大buffer存数据, 多次draw)
2.创建ShapeNode, 利用MemoryMeshLoader加载1中的数据到实例中, 同时设置shader和相应参数(纹理也是shader 参数的一种, 渲染状态是包含在fx中的, 所以也属于shader)
3.创建ManagedModel, 创建一个Model, 把Model设置给ManagedModel. 然后把2中的ShapeNode Attach到Model, 并利用一个EmptyResourceLoader来完成资源状态的切换(因为数据已经有了, 需要把资源状态切换到”加载完成”才能使用)
4.创建InternalModelEntity, 利用SetManagedModel把3的结果给塞进去
5.InternalGraphicsServer::Instance()->GetDefaultView()->GetStage()得到默认的场景后, 把InternalModelEntity通过AttachEntity加入场景, 之后就不用操心了. 场景管理器会自己去画. 
知道了这些, 写个自定义模型格式的ModelLoader就很容易了, 嘿嘿
不知道把Terrain Tile当成ModelEntity可不可行, 这样的话连摄像机裁剪都省了-_-. 能需要特殊的ResourceMapper来管理地形的LOD…嗯, 如果这个思路可行的话, 地形完全可以做成addon
