# 【OpenGL4.0】GLSL-Flat Shading平面着色 - 江南烟雨 - CSDN博客
2012年06月13日 20:34:49[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：6323
**Per-vertex**渲染技术是针对每个顶点进行渲染计算，然后把计算得到的颜色值和这个顶点关联起来。然后在多边形的面上进行颜色插值以后的平滑的渲染效果。这也叫做**Gouraud Shading**。在早起的OpenGL版本中，它是默认的渲染技术。
有的时候我们需要达到遮掩过一种效果：一个多边形上只有一种颜色，而不是有这种插值方法得到的平滑效果。这个时候的渲染就叫做**Flat Shading**。
下面的图显示两种渲染效果的对比：
![](https://img-my.csdn.net/uploads/201206/13/1339590612_8934.jpg)
**Gouraud Shading**
![](https://img-my.csdn.net/uploads/201206/13/1339590618_8542.jpg)
**Flat Shading**
在早起的OpenGL版本中，Flat shading效果是通过下列函数调用实现的：
*glShadeModel(GL_FLAT)。*
而且，当前多边形面所使用的颜色是这个多边形的顶点中最后一个被渲染的顶点的颜色。
在OpenGL4.0中，flat shading效果可以通过对着色器的输入和输出变量使用一个修饰符**flat**很方便的实现。
在顶点着色器的输出变量和片断着色器要使用作为颜色的输入变量前使用这个修饰符即可。
这个修饰符表明了这个值在传递到片断着色器的时候没有**插值**发生。
最后多边形的颜色可能是其顶点中最先或最后被渲染的顶点的颜色。可以通过下列函数调用来控制：
*glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);*
或者是
*glProvokingVertex(GL_LAST_VERTEX_CONVENTION);*
*参考资料：*
*[http://www.futuretech.blinkenlights.nl/gouraud.html](http://www.futuretech.blinkenlights.nl/gouraud.html)*
[http://graphics.wikia.com/wiki/Flat_shading](http://graphics.wikia.com/wiki/Flat_shading)
[http://en.wikipedia.org/wiki/Gouraud_shading](http://en.wikipedia.org/wiki/Gouraud_shading)
[http://www.cs.cmu.edu/~fp/courses/02-graphics/pdf-color/08-shading.pdf](http://www.cs.cmu.edu/~fp/courses/02-graphics/pdf-color/08-shading.pdf)
