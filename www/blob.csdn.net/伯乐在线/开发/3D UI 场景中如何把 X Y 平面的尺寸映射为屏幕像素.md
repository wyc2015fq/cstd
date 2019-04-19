# 3D UI 场景中如何把 X Y 平面的尺寸映射为屏幕像素 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [Plato](http://www.jobbole.com/members/Plato) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
之前用kanzi的3D UI引擎和cocos-2d的时候都有遇到过这个问题，就如何把3D场景中的XY平面的尺寸映射为与屏幕像素一一对应的，即XY平面上的一个单位对应平面上的一个像素。这个在3D UI开发过程中似乎并非必须，或者说很少有人这样用，因为在游戏场景中，UI可以处于场景的任何位置，并不局限于XY平面内。
本次的分享总结所述的3D UI应用场景并非在游戏中，而是注重在GUI应用上（类似QT等），即使用3D绘图技术实现的一套类似2D UI一样效果的引擎，由于UI系统是3D的，故能实现3D的动画效果。把3D场景中的XY平面的尺寸映射为平面像素一一对应的优点，是能保持并延续我们在2D开发时候的习惯，方便精准地控制UI控件在整个屏幕上的位置布局。
本文的重点是“3D UI场景中把XY平面的尺寸映射为屏幕像素”，因此需要您有如下的基本知识：
1、基本3D数理知识；
2、Opengl相关知识；
3、对3D计算机图形学中“摄像机”概念有所了解；
本文包括如下内容：
-  glm 3D数学库简介；
-  透视视锥体介绍；
-  使用glm函数库生成摄像机矩阵；
-  分析如何调整摄像机和透视视锥体，使的3D场景中的XY平面的尺寸与屏幕像素对应；
# glm 3D 数学库简介
什么是3D数学库？
所谓3D数学库，简单地说就是把在3D计算机编程中常用到的数据类型、数学函数、3D处理公式及方法等统一集中起来，方便我们在处理3D场景时使用。
glm 3D数学库是Opengl官网推荐使用的，包含了几乎所有我们在处理3D场景是需要的数学函数。
glm的使用也非常简单，glm提供的源码全部都是头文件，我们只需把glm的头文件引用到自己需要使用的工程中即可。
如下实例代码中，我们通过glm创建了一个4×4的矩阵，并对该矩阵进行了平移变换（详细的glm使用介绍，大家可以参考glm官网的教程或文档）。

```
//示例代码 1.0 http://www.cnblogs.com/feng-sc/
#include //注意： glm的工程路径需要自己配置
#include int main()
{
    glm::mat4 matrix(1.0);
    matrix = glm::translate(matrix, glm::vec3(100.0f, 0.0f, 0.0f));
    return 0;
}
```
作为简介，glm的介绍就到此结束。
# 透视视锥体介绍
所谓的透视不是你所想的眼睛能看穿墙的意思，别多想了！简单点，透视就是表示物体近大远小的效果的意思。
如下图所示，透视视锥体梯体几何图形，它类似于人的眼睛所能看到的范围，在梯体之外的物体将不可见。
在3D数学里，用什么表示这个透视视锥体呢？没错，是矩阵！
使用glm函数库能简单地生成透视视锥体的矩阵，如下实例代码：

```
// 示例代码1.0： www.cnblogs.com/feng-sc/
// fovyInRadians : 弧度表示下图中FOV
// aspect        : 视锥体宽与高的比例，可以理解为绘图区域的宽高比
// zNear         : 近平面离摄像机的距离
// zFar          ：远平面离摄像机的距离
glm::mat4 projection = glm::perspective(fovyInRadians, aspect, zNear, zFar);
```
![](http://jbcdn2.b0.upaiyun.com/2016/08/ec38d5a60c5ffbce097e5ef02272058b.png)
（透视视锥体）
上诉实例代码中，projection又被成为透视矩阵，所有3D世界里的物体，经过与projection矩阵相乘后，最终得到的物体将呈现如下两种特点：
1、远小近大的效果；
2、处于透视视锥外的物体将被忽略；
# 使用 glm 函数库生成摄像机矩阵
本段我们先以一段代码起头，如下：

```
// 示例代码1.0： www.cnblogs.com/feng-sc/
glm::mat4 view = glm::lookAt(m_position, m_target, m_up);
```
lookAt函数得到的结果是一个视图矩阵。有人把视图矩阵称为摄像机，也有人把视图矩阵和透视投影矩阵合在一起称为摄像机，我喜欢后者。
结合投影矩阵，我们总结一下，摄像机分别由如下参数决定：
1、透视投影矩阵projection决定了摄像机的视野范围，包括视觉张角FOV、近平面、远平面；
2、视图矩阵决定了摄像机的位置、观察方向；
最后投影矩阵与视图矩阵将共同决定我们整个场景的显示效果。

```
// 示例代码1.0 www.cnblogs.com/feng-sc/
glm::mat4 projection = glm::perspective(fovyInRadians, aspect, zNear, zFar);
glm::mat4 view = glm::lookAt(m_position, m_target, m_up);
glm::mat4 vpMat = projection * view
```
# 分析如何调整摄像机和透视视锥体，使的3D场景中的XY平面的尺寸与屏幕像素对应；
OK，终于来到了本文标题讨论的问题点，3D UI场景中把XY平面的尺寸映射为屏幕像素。
其实到现在为止，我们问题的解决方案也清晰了，如何实现“3D UI场景中把XY平面的尺寸映射为屏幕像素” 呢？是的，就是调整摄像机的位置、远/近平面、摄像机视角，使XY平面的单位尺寸恰好与平面像素的单位对应即可。
那么现在剩下的问题是：**如何调整摄像机，使得我们的XY平面恰好与平面像素对应呢？**
在我们继续之前，我们先来了解一个概念：**齐次坐标。**
**百度百科解释说：**齐次坐标就是将一个原本是n维的向量用一个n+1维向量来表示。例如，二维点(x,y)的齐次坐标表示为(hx,hy,h)（h可以是任意值）。
我们可以理解为，任何三维的点(hx,hy,h)，在二维平面上的投影点均为（x，y）。
**![](http://jbcdn2.b0.upaiyun.com/2016/08/6daac5badd74e8a08d0bff9d2fa5ab58.png)**
（透视视锥体侧面平面图）
上图为透视视锥体侧面平面图，其中GI为透视视锥体的近平面，BF为远平面，LS和TZ分别为视锥体的两个不同位置的截面。
从2D平截视锥体看，透视视锥体GBFI范围内的三维物体最后均被投影到GI平面上。由齐次坐标概念可知，点B、U、M在GI平面上的投影均为点M，同理点F、W、P在GI平面上的投影均为点I。
我们：
假设TZ平面为XY平面且与屏幕像素对应，屏幕高度像素为h，角∠BAF = FOV (FOV为摄像机张角)
故，UW = h，UV = h/2;
故，
![](http://jbcdn2.b0.upaiyun.com/2016/08/35713496b216dfb31154c1ed5eb0ef9e.jpg)
即，由屏幕宽度和摄像机张角，要使XY平面与屏幕像素对应，我们求得摄像机位置点距离XY屏幕距离长度为AV。
下面的代码设置为屏幕左上角为原点是，摄像机的设置。

```
// 示例代码1.0 www.cnblogs.com/feng-sc/
float fov = 60;
glm::perspective(glm::radiansfloat>(fov), (float)width / (float)height, 0.1f, 10000.0f);
float z = height / (2 * tan(((float)(fov / 2.0)* glm::pifloat>()) / 180.0));
glm::vec3 positon((float)width / 2.0f, (float)height / 2.0f, -z);
glm::vec3 target((float)width / 2.0f, (float)height / 2.0f, 0.0f);
glm::vec3 up(0.0f, -1.0f, 0.0f);
m_view = glm::lookAt(positon target, up);
```
