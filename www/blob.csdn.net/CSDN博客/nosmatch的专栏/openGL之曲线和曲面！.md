# openGL之曲线和曲面！ - nosmatch的专栏 - CSDN博客
2010年03月27日 15:35:00[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：912标签：[callback																[网格																[存储																[buffer																[图形																[编程](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=图形&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=网格&t=blog)](https://so.csdn.net/so/search/s.do?q=callback&t=blog)
     计算机图形学中，所有的光滑曲线、曲面都采用线段或三角形逼近来模拟，但为了精确地表现曲线，通常需要成千上万个线段或三角形来逼近，这种方法对于计算机的硬件资源有相当高的要求。然而，许多有用的曲线、曲面在数学上只需要用少数几个参数（如控制点等）来描述。这种方法所需要的存储空间比线段、三角形逼近的方法来所需要的空间要小得多，并且控制点方法描述的曲线、曲面比线段、三角形逼近的曲线、曲面更精确。
　　为了说明如何在OpenGL中绘制复杂曲线和曲面，我们对上述两类比方法都进行了介绍。下面我们先来介绍有关基础知识，然后再看是如何实现的吧。
**一、曲线的绘制**
　　OpenGL通过一种求值器的机制来产生曲线和曲面，该机制非常灵活，可以生成任意角度的多项式曲线，并可以将其他类型的多边形曲线和曲面转换成贝塞尔曲线和曲面。这些求值器能在任何度的曲线及曲面上计算指定数目的点。随后，OpenGL利用曲线和曲面上的点生成标准OpenGL图元，例如与曲线或曲面近似的线段和多边形。由于可让OpenGL计算在曲线上所需的任意数量的点，因此可以达到应用所需的精度。
对于曲线，OpenGL中使用glMap1*（）函数来创建一维求值器，该函数原型为：
void glMap1{fd}(GLenum target,TYPE u1,TYPE u2,GLint stride, GLint order,const TYPE *points);
　　函数的第一个参数target指出控制顶点的意义以及在参数points中需要提供多少值，具体值见表一所示。参数points指针可以指向控制点集、RGBA颜色值或纹理坐标串等。例如若target是GL_MAP1_COLOR_4，则就能在RGBA四维空间中生成一条带有颜色信息的曲线，这在数据场可视化中应用极广。参数u1和u2，指明变量U的范围，U一般从0变化到1。参数stride是跨度，表示在每块存储区内浮点数或双精度数的个数，即两个控制点间的偏移量，比如上例中的控制点集ctrpoint[4][3]的跨度就为3，即单个控制点的坐标元素个数。函数参数order是次数加 1，叫阶数，与控制点数一致。
|参数|意义|
|----|----|
|GL_MAP1_VERTEX_3|x,y,z顶点坐标|
|GL_MAP1_VERTEX_4|x,y,z,w 顶点坐标|
|GL_MAP1_INDEX|颜色表|
|GL_MAP1_COLOR_4|R,G,B,A|
|GL_MAP1_NORMAL|法向量|
|GL_MAP1_TEXTURE_COORD_1|s　纹理坐标|
|GL_MAP1_TEXTURE_COORD_2|s,t　纹理坐标|
|GL_MAP1_TEXTURE_COORD_3|s,t,r　纹理坐标|
|GL_MAP1_TEXTURE_COORD_4|s,t,r,q　纹理坐标|
　　　　　　　　　　　　　　表一、参数target的取值表
　　使用求值器定义曲线后，必须要启动求值器，才能进行下一步的绘制工作。启动函数仍是glEnable()，其中参数与glMap1*()的第一个参数一致。同样，关闭函数为glDisable()，参数也一样。
　　一旦启动一个或多个求值器，我们就可以构造近似曲线了。最简单的方法是通过调用计算坐标函数glEvalcoord1*()替换所有对函数glVertex*()的调用。与glVertex*()使用二维、三维和四维坐标不同，glEvalcoord1*()将u值传给所有已启动的求值器，然后由这些已启动的求值器生成坐标、法向量、颜色或纹理坐标。OpenGL曲线坐标计算的函数形式如下：
void glEvalCoord1{fd}[v](TYPE u);
　　该函数产生曲线坐标值并绘制。参数u是定义域内的值，这个函数调用一次只产生一个坐标。在使用glEvalCoord1*()计算坐标，因为u可取定义域内的任意值，所以由此计算出的坐标值也是任意的。
　　使用glEvalCoord1*()函数的优点是，可以对U使用任意值，然而，如果想对u使用N个不同的值，就必须对glEvalCoord1*()函数执行N次调用，为此，OpenGL提供了等间隔值取值法，即先调用glMapGrid1*()定义一个间隔相等的一维网格，然后再用glEvalMesh1()通过一次函数执行，将求值器应用在网格上，计算相应的坐标值。下面详细解释这两个函数：
　　1、void glMapGrid1{fd}(GLint n,TYPE u1,TYPE u2);
　　定义一个网格，从u1到u2分为n步，它们是等间隔的。实际上，这个函数定义的是参数空间网格。
　　2、void glEvalMesh1(GLenum mode,GLint p1,GLint p2);
　　计算并绘制坐标点。参数mode可以是GL_POINT或GL_LINE，即沿曲线绘制点或沿曲线绘制相连的线段。这个函数的调用效果同在p1和p2之间的每一步给出一个glEvalCoord1()的效果一样。从编程角度来说，除了当i=0或i=n，它准确以u1或u2作为参数调用glEvalCoord1()之外，它等价于一下代码：
glBegin(GL_POINT);　/* glBegin(GL_LINE_STRIP); */
　　for(i=p1;i<=p2;i++)
　　　　glEvalCoord1(u1+i*(u2-u1)/n);
　　glEnd();
　　为了进一步说明OpenGL中曲线的绘制方法。下面我们来看一个简单的例子，这是用四个控制顶点来画一条三次Bezier曲线。程序如下（注：这是本讲座中提供的第一个完整的OpenGL实例代码，如果读者朋友对整个程序结构有些迷惑的话，也不要紧，慢慢地往下看，先有一个感官上的印象，主要是掌握如何实现曲线绘制这一部分。关于OpenGL的程序整体结构实现，笔者将在第五讲中专门阐述）：
#include "glos.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
void myinit(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK display(void);
GLfloat ctrlpoints[4][3] = {
　　{ -4.0, -4.0, 0.0 }, { -2.0, 4.0, 0.0 },
　　{ 2.0,　-4.0, 0.0 }, { 4.0,　4.0, 0.0 }
};
void myinit(void)
{
　　glClearColor(0.0, 0.0, 0.0, 1.0);
glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4,
&ctrlpoints[0][0]);
　　glEnable(GL_MAP1_VERTEX_3);
　　glShadeModel(GL_FLAT);
}
void CALLBACK display(void)
{
　　int i;
　　glClear(GL_COLOR_BUFFER_BIT);
　　glColor3f(1.0, 1.0, 1.0);
　　glBegin(GL_LINE_STRIP);
　　for (i = 0; i <= 30; i++)
　　　　　glEvalCoord1f((GLfloat) i/30.0);
　　glEnd();
　　/* 显示控制点 */
　　glPointSize(5.0);
　　glColor3f(1.0, 1.0, 0.0);
　　glBegin(GL_POINTS);
　　for (i = 0; i < 4; i++)
　　　　glVertex3fv(&ctrlpoints[i][0]);
　　glEnd();
　glFlush();
}
void CALLBACK myReshape(GLsizei w, GLsizei h)
{
　glViewport(0, 0, w, h);
　glMatrixMode(GL_PROJECTION);
　glLoadIdentity();
　if (w <= h)
glOrtho(-5.0, 5.0, -5.0*(GLfloat)h/(GLfloat)w,
5.0*(GLfloat)h/(GLfloat)w, -5.0, 5.0);
　else
glOrtho(-5.0*(GLfloat)w/(GLfloat)h, 5.0*(GLfloat)w/(GLfloat)h, -5.0, 5.0, -5.0, 5.0);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
　　}
