# osg笔记四 - L_Andy的专栏 - CSDN博客

2018年11月23日 22:01:18[卡哥](https://me.csdn.net/L_Andy)阅读数：415



![](https://img-blog.csdnimg.cn/20181123215825593.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0xfQW5keQ==,size_16,color_FFFFFF,t_70)

**osg****模型发黑**

简述

开发过程中遇到模型发黑。一般情况是光照和模型的法线设置不正确。

    (1)osg中光照只会对有法线的模型起作用，而模型经过缩放后法线是不会变得，所以需要手动设置属性，让法线随着模型大小变化而变化。node->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);

    (2)有些时候可能是光照的问题，有个笨办法是直接把模型的光照关闭

    node->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF| StateAttribute::OVERRIDE);

    (3)如果模型始终是一面发黑一面正常，可能是只有平行光，需要设置个全局光。

//////////////////////////////////////////////////////////////////////////////////////////////////////

/LARGEADDRESSAWARE（处理大地址）
`/LARGEADDRESSAWARE[:NO]  `
## **备注**

/LARGEADDRESSAWARE 选项通知链接器应用程序可以处理大于 2 GB 的地址。 在 64 位编译器中，默认情况下启用此选项。 在 32 位编译器中，如果未在链接器行上指定 /LARGEADDRESSAWARE，则将启用 /LARGEADDRESSAWARE:NO。

如果用 /LARGEADDRESSAWARE 来链接应用程序，则 DUMPBIN [/HEADERS](https://msdn.microsoft.com/zh-cn/library/9ha900t8.aspx)将显示该效果的信息。

### **在 Visual Studio 开发环境中设置此链接器选项**
- 打开项目的**“属性页”**对话框。 有关详细信息，请参见[设置 Visual C++ 项目属性](https://msdn.microsoft.com/zh-cn/library/669zx6zc.aspx)。
- 单击“链接器”文件夹。
- 单击“系统”属性页。
- 修改“启用大地址”属性。

////////////////////////////////////////////////////////////////////////////////////////

Globl Map编辑矢量线

Alt + D进行选择和删除

GloblMap裁切高程数据范围

![](https://img-blog.csdnimg.cn/20181123215955135.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0xfQW5keQ==,size_16,color_FFFFFF,t_70)

变为

![](https://img-blog.csdnimg.cn/20181123220033298.png)

////////////////////////////////////////////////////////////////////////////////////////

SetRenderBinDetails(负数与正数有区别吗？ “RenderBin”);

数值越小，这个渲染元renderbin越先渲染，

为啥要用负数，0是谁的渲染

0是普通渲染元，就是没有显示设置setrenderbindetail的普通stateset

root->node0

root->node1(node1->getorcreatestateset()->setrenderbindetail(1));

root->node2(node2->getorcreatestateset()->setrenderbindetail(-1));

node0没有进行设置，其默认就是0

上述三节点的渲染顺序node2,node0,node1

////////////////////////////////////////////////////////////////////////////////////////

//_xform得到地形自身坐标系，可直接使用经纬度

_xform = new GeoTransform();

_xform->setTerrain(mapnode->getTerrain());

_pat通过位置和四元数设置一个坐标转换

//_pat = new osg::PositionAttitudeTransform();                                                       

_pat->addChild(_model);

//////////////////////////////////////////////////////////////////////////////////////

double t =*fmod*(view->*getFrameStamp*()->*getSimulationTime*(),

(double)g_duration.*get*())/(double)g_duration.*get*();

fmod是函数 原型为float fmod(float x, float y);

%是运算符，只能用于整数运算。

此函数求得t的取值在【0,1】之间的数。

////////////////////////////////////////////////////////////////////////////////////////

//设置自动生成纹理坐标

osg::ref_ptr<osg::TextGen > texgen = new osg::TexGen();

texgen->setMode(osg::TexGen::SPHERE_MAP);

////////////////////////////////////////////////////////////////////////////////////////

即Cg profile 定义了一个“被特定图形硬件或API所支持的Cg语言子集”。

在《实时计算机图形学》一书中，将图形绘制管线分为三个主要阶段：应用程序阶段、几何阶段、光栅阶段。

什么是片段，片段和像素有什么不一样？所谓片段就是所有的三维顶点在光栅化之后的数据集合，这些数据还没有经过深度值比较，而屏幕显示的像素都是经过深度比较的。

////////////////////////////////////////////////////////////////////////////////////////

对所有顶点操作的矩阵叫做模型矩阵，模型矩阵所完成的工作就是对点进行缩放、平移、旋转等操作。

、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、

注意，在表面的一个特定的点上，有两条向量垂直于这个表面，它们指向相反的方向。按照约定指向表面外侧的那条向量就是它的法线。如果想反转模型的内侧和外侧，只要把所有的法线向量从（x，y,  z）修改为（-x,-y,-z）就可以了。

、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、

另外需要记住的是，由于法线向量只表示方向，因此它的长度是无关紧要的，法可以指定为任意长度，但是在执行光照计算之前，它的长度会转换为1（长度为1的向量称为单位向量或规范化的向量）。一般而言，我们应该提供规范化的法线向量。为了使一条法线向量具有单位长度，只要把它的每个x、y和z成分除以法线的长度就可以了。

                              法线的长度 =![](https://img-blog.csdnimg.cn/20181123215914414.png)

如果模型变换只涉及旋转和移动，法线向量就能保持它的规范化。如果进行了不规则变换（例如进行了缩放或者乘以了剪切矩阵），或者指定了非单位长度的法线，那么在经过变换之后，OpenGL会自动对法线向量进行规范化。为了启用这个功能，可以调用glEnable(GL_NORMALIZE)。

过滤：详细解释当纹理应用于多边形的像素时是如何放大或缩小的。

////////////////////////////////////////////////////////////////////////////////////////////////

osg::LightModel->setTwoSided(true)

///////////////////////////////////////////////////////////////////////////////////

顶点着色器可以用于替代顶点和法线变换、纹理坐标生成和变换、光照以及材质应用这些传统的管线命令；片元着色器则用于替代纹理应用、雾化和像素汇总的工作；几何着色器则可以在图元装配的过程中重新生成新的图元。

//////////////////////////////////////////////////////////////////////////////////////////////////

Deep Exploration 3D文件格式转换软件

/////////////////////////////////////////////////////////////////////////////////////////////////

# osgearth消除近裁剪平面离物体太近时的裁剪问题

// This will mitigate near clip plane issues if you zoom in close to the ground:

    LogarithmicDepthBuffer buf;

    buf.install( viewer.getCamera() );

