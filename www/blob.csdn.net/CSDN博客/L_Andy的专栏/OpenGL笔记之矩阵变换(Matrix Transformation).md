# OpenGL笔记之矩阵变换(Matrix Transformation) - L_Andy的专栏 - CSDN博客

2015年08月04日 15:00:22[卡哥](https://me.csdn.net/L_Andy)阅读数：891
个人分类：[OpenGL](https://blog.csdn.net/L_Andy/article/category/2817801)



本文是学习OpenGL过程中的一篇笔记。在学习过程中，主要参照了大名鼎鼎的

- 《OpenGL Programming Guide》（中文名《OpenGL编程指南》，有些人还称之为OpenGL红宝书）
- 《OpenGL SuperBible》（中文名《OpenGL超级宝典》，有些人还称之为OpenGL蓝宝书）

另外还有

- 《3D Computer Graphics: A Mathematical Introduction with OpenGL》
- 《Advanced Graphics Programming Using OpenGL Morgan Kaufmann Series in Computer Graphics and Geometric Modeling 》
- 《3D Math Primer for graphics and game development》

等书籍。

![](https://img-my.csdn.net/uploads/201208/06/1344219987_3381.png)
![](https://img-my.csdn.net/uploads/201208/06/1344224931_8887.png)

1，应用程序传送给OpenGL的顶点位置坐标，法线向量坐标、纹理坐标以及光源坐标都是定义在**对象坐标系**中的。如果应用程序开启了基于对象坐标系的纹理生成，那么这些纹理坐标的生成是利用末经模型视图矩阵变换的顶点位置坐标生成的，所以这种情况下如果移动或者旋转物体，纹理将和物体一起移动或者旋转，实例请参考蓝宝书第9章的TEXGEN程序。

The vertices of a model are typically stored in **object space**, a coordinate system that is local to the particular model and used only by that model. The position and orientation of each model are often stored in**world space**, a global coordinate system that ties all of the object spaces together. Before  an object can be rendered, its vertices must be transformed into  **camera space** (also called  eye space), the space in which the x and y axes are
 aligned to the display and the z axis is parallel to the viewing direction. 

2，模型转换（Modeling Transformation）：使用模型转换的目的是设置物体在场景中的位置（position）和方向（orientation）。通常，为了正确的在场景中放置每个物体，每个物体都需要一个不同的模型转换。如果要使物体产生运动效果，也需要在每帧更新时对物体使用不同的模型转换。

3，视觉转换（Viewing Transformation）：使用视觉转换的目的是改变场景中所有物体的位置和方向，换句话说，就是改变观察的位置和方向。实例请参见下图：

![](https://img-blog.csdn.net/20131110215335656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW15ZnJpZW5k/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

视觉转换需要的信息是viewer’s (eye) position, the center of observation, and the normal (“vector” quantity) to the viewer’s head (as indicated by the orange arrow over the Blender camera in Figures。

4，使用glLight命令给定光源位置和方向时，在光源位置和方向被OpenGL存储之前会经过模型视图矩阵（ModelView Matrix）的变换。如果没有用glLight命令更新的话，这些位置和方向将一直被OpenGL在计算光照时使用。

5，在视觉坐标系中，物体光照效果发生作用，基于视觉坐标系的纹理坐标生成也是发生在视觉坐标系中的。默认情况下，视点（或者说成照相机）位于视觉坐标系原点，指向Z轴负方向，向上向量为（0,1,0），请参见下图。

![](https://img-my.csdn.net/uploads/201208/03/1343988257_8328.png)

由于视觉坐标系指向Z轴负方向，所以glFrustum(GLdouble left,GLdouble  right,GLdouble bottom,GLdouble 
 top,GLdouble  near,GLdouble  far)和gluPerspective(GLdouble fovy,GLdoubleaspect,GLdoublenear,GLdoublefar)中的near和far是在以Z轴负方向的情况下指定的。

可以直接使用gluLookAt来改变视点的位置和方向。注意，在OpenGL代码中，视觉转换一定要发生在模型转换之前，可以在绘图之前的任何时刻进行投影变换和视口变换。

如果使能光照的话，openGL会在视觉坐标系中使用视点位置，光源位置和法线向量来进行光照计算，从而修改物体顶点的颜色。为了便于理解，这里引用一个光照模型图，如下：

![](https://img-my.csdn.net/uploads/201208/03/1343990847_4554.png)

6，投影变换（Project Transformation）：投影变换的一个显著特征是透视缩短，即物体如果离照相机的距离越远，则它看上去就越小。透视投影可以看成一个金字塔的平截头体。靠近观察点的物体看上去会更大一些，因为和远处的物体相比，他们占据了视景体中相对较大的区域。

![](https://img-my.csdn.net/uploads/201208/06/1344221833_8729.png)

投影变换其实是对对象进行变形处理，使得变形后的对象经正交投影后得到与原对象的理想投影后一样的视图（详细请参见：[投影矩阵](http://wenku.baidu.com/view/76b11733b90d6c85ec3ac6a6.html#%23%23)，[深入探索透视投影变换](http://wenku.baidu.com/view/72fe7bf7ba0d4a7302763a14.html#%23%23)）。

       “透视”一词源自拉丁文“perspclre”，意为看透。为了将立体物体的图像转化到平面上以完成作画的过程，人们开始了关于透视的研究。最初研究透视是采取通过一块透明的平面去看景物的方法，将所见景物准确描画在这块平面上，即成该景物的透视图。随着研究的深入，人们将在平面画幅上根据一定原理，用线条来显示物体的空间位置、轮廓和投影的科学称为透视学。

       在多种透视方法中，最常用到的是线透法。线透法的基本原理，在于将物体的每一个点全部以连线的方式与观察视野，也就是我们的眼睛相连，然后再将一块平面置于这些线上，由线穿透平面所构成的投影来确定物体在画面中的形态。无论是现代西方美术、摄影还是图形渲染工业，一切与现代西方美术有关并以平面化展现立体物体形象为任务的领域，都要依赖线透法来实现最终的画面效果。

透视投影在图形学中非常重要，因为它是人类视觉系统的模型。为便于理解，我们以小孔成像来说明它的基本原理。请参见下图：

![](https://img-my.csdn.net/uploads/201208/06/1344222247_8860.png)

图中右边的茶壶经过小孔投影到左边的立方体中。其剖面图如下：

![](https://img-my.csdn.net/uploads/201208/06/1344222395_7178.png)

为了计算上的方便，我们将投影平面移到投影的前面，如下图：

![](https://img-my.csdn.net/uploads/201208/06/1344222499_8473.png)

这就是OpenGL使用的投影原理。

我们可以用glFrustum建立投影视景体，如下图：

![](https://img-my.csdn.net/uploads/201208/06/1344222789_7416.png)

此时，投影后所形成的最终图像将会显示在near平面上。投影矩阵的推导过程请参见《[OpenGL
 Projection Matrix](http://www.songho.ca/opengl/gl_projectionmatrix.html)》。

注意：投影变换变换后的坐标系是右手坐标系，即点（0,0,1）比点（0,0,-1）距离观察者近。

7，透视除法（Perspective Division）：通过透视除法，我们可以得到NDC（Normalized Device Coordinate）。规范化的原因是不想为每种类型的投影设计不同的投影矩阵，所以把所有的投影转换为具有默认视景体的正交投影。

通过投影变换和透视除法，我们得到NDC。这个过程可能难于理解，下面我们用计算机图形学中的一点透视来解释一下（详细请参见《[三维变换及三维观察](http://wenku.baidu.com/view/e5833625482fb4daa58d4bda.html#%23%23)》）。我们考虑从NDC到视景体的变换，步骤如下：

设：视点（投影中心）在Z轴上（z=-d），投影面在XOY上，一点透视的步骤：

①平移三维形体到（l、m、n）

②确定d的值，透视变换

③向XOY作正投影变换

![](https://img-my.csdn.net/uploads/201208/06/1344233725_6556.png)

![](https://img-my.csdn.net/uploads/201208/06/1344233893_8027.png)

8，Normalized Device Coordinate System (NDCS) is the coordinate system in which the entire screen of the device corresponds to a unit cube, so all points in this cube are within the range [1, 1, 1] to [-1, -1, -1] for x, y, and z。

![](https://img-blog.csdn.net/20131110220812890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW15ZnJpZW5k/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

注意：NDC坐标系是左手坐标系，即点（0,0,-1）比点（0,0,1）距离观察者近。

可以通过在启用深度测试（GL_DEPTH_TEST）后，使用glDepthRangef(1, 0) 来将左手坐标系变成右手坐标系。

9，窗口变换矩阵（Viewport Transformation）：在NDC视景体中的每一个点（x，y，z），其x，y，z取值范围都在区间[-1,1]内。它的x和y坐标映射到一个以（Vx，Vy）为起点，宽为w，高为h，中心点为（Vx+w/2, Vy+y/2）的窗口（Viewport）中。z坐标默认映射到范围[0,1]中，也可改变它，使它映射到[Dn,Df]中，这样深度范围d=Df-Dn，并且它的中心点是(Dn+Df)/2。这样，实现从NDC到窗口坐标转换的窗口变换矩阵是：

![](https://img-my.csdn.net/uploads/201208/06/1344226229_4227.png)

下面是2张效果示例图：

![](https://img-my.csdn.net/uploads/201208/06/1344232770_7448.png)

![](https://img-my.csdn.net/uploads/201208/06/1344232780_9274.png)

10，关于红宝书中3.2节**视图和模型转换**中提到的**局部移动坐标系**可以参考《3D Math Primer for graphics and game development》3.5节**Coordinate Space Transformations**进行理解。

