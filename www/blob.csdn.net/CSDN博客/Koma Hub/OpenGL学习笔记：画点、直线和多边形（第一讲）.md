# OpenGL学习笔记：画点、直线和多边形（第一讲） - Koma Hub - CSDN博客
2018年05月05日 12:12:45[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：1138
我的***运行环境：***
```
CentOS7
g++ (GCC) 4.8.5 20150623 (Red Hat 4.8.5-16)
OpenGL实现厂商的名字：VMware, Inc.
渲染器标识符：Gallium 0.4 on llvmpipe (LLVM 3.9, 256 bits)
OpenGL实现的版本号：2.1 Mesa 17.0.1
OGLU工具库版本：1.3
```
在OpenGL中指定顶点，OpenGL提供了一系列函数。它们都以glVertex开头，后面跟一个数字和1~2个字母。
例如： 
```
glVertex2d 
glVertex2f 
glVertex3f 
glVertex3fv
```
数字表示参数的个数，2表示有两个参数，3表示三个，4表示四个（我知道有点罗嗦~）。 
字母表示参数的类型，
s表示16位整数（OpenGL中将这个类型定义为GLshort）， 
i表示32位整数（OpenGL中将这个类型定义为GLint和GLsizei）， 
f表示32位浮点数（OpenGL中将这个类型定义为GLfloat和GLclampf）， 
d表示64位浮点数（OpenGL中将这个类型定义为GLdouble和GLclampd）。 
v表示传递的几个参数将使用指针的方式，见下面的例子。 
这些函数除了参数的类型和个数不同以外，功能是相同的。
例如，以下五个代码段的功能是等效的： 
```cpp
（一）glVertex2i(1, 3); 
（二）glVertex2f(1.0f, 3.0f); 
（三）glVertex3f(1.0f, 3.0f, 0.0f); 
（四）glVertex4f(1.0f, 3.0f, 0.0f, 1.0f); 
（五）GLfloat VertexArr3[] = {1.0f, 3.0f, 0.0f}; 
      glVertex3fv(VertexArr3);
```
假设现在我已经指定了若干顶点，那么OpenGL是如何知道我想拿这些顶点来干什么呢？是一个一个的画出来，还是连成线？或者构成一个多边形？或者做其它什么事情？ 为了解决这一问题，OpenGL要求：指定顶点的命令必须包含在glBegin函数之后，glEnd函数之前（否则指定的顶点将被忽略）。并由glBegin来指明如何使用这些点。 
例如：
```cpp
glBegin(GL_POINTS); 
glVertex2f(0.0f, 0.0f); 
glVertex2f(0.5f, 0.0f); 
glEnd();
```
则这两个点将分别被画出来。
如果将GL_POINTS替换成GL_LINES，则两个点将被认为是直线的两个端点，OpenGL将会画出一条直线。 我们还可以指定更多的顶点，然后画出更复杂的图形。 
另一方面，glBegin支持的方式除了GL_POINTS和GL_LINES，还有GL_LINE_STRIP，GL_LINE_LOOP，GL_TRIANGLES，GL_TRIANGLE_STRIP，GL_TRIANGLE_FAN等，每种方式的大致效果见下图：
![](https://img-blog.csdn.net/20180505120742235)
声明：该图片来自www.opengl.org，该图片是《OpenGL编程指南》一书的附图，由于该书的旧版（第一版，1994年）已经流传于网络，我希望没有触及到版权问题。
下面简单给出***源代码：***
```cpp
/**
 * 《OpenGL入门教程》
 *               点、直线和多边形
 */
#include <GL/glut.h> 
#include <math.h>
const int n = 200; 
const float R = 0.5; 
const float Pi = 3.1415926536;
/*画一个圆*/
void myDisplay1(void) 
{ 
    int i; 
    float x,y;
    glClear(GL_COLOR_BUFFER_BIT); 
    glBegin(GL_POLYGON); 
    for(i=0; i<n; ++i) 
    {
        x = R*cos(2*Pi/n*i);
        y = R*sin(2*Pi/n*i);
        glVertex2f(x, y); 
    }
    glEnd(); 
    glFlush();
}
/*画一个五角星*/
/* 首先，根据余弦定理列方程，
计算五角星的中心到顶点的距离a （假设五角星对应正五边形的边长为.0） 
a = 1 / (2-2*cos(72*Pi/180)); 
然后，根据正弦和余弦的定义，计算B的x坐标bx和y坐标by，
以及C的y坐标 （假设五角星的中心在坐标原点） 
bx = a * cos(18 * Pi/180); 
by = a * sin(18 * Pi/180); 
cy = -a * cos(18 * Pi/180); 
五个点的坐标就可以通过以上四个量和一些常数简单的表示出来 */
void myDisplay2(void) 
{ 
    GLfloat a = 1 / (2-2*cos(72*Pi/180)); 
    GLfloat bx = a * cos(18 * Pi/180); 
    GLfloat by = a * sin(18 * Pi/180); 
    GLfloat cy = -a * cos(18 * Pi/180); 
    GLfloat PointA[2] = { 0, a },
            PointB[2] = { bx, by }, 
            PointC[2] = { 0.5, cy }, 
            PointD[2] = { -0.5, cy }, 
            PointE[2] = { -bx, by };
    glClear(GL_COLOR_BUFFER_BIT);
    // 按照A->C->E->B->D->A的顺序，可以一笔将五角星画出 
    glBegin(GL_LINE_LOOP);
    glVertex2fv(PointA); 
    glVertex2fv(PointC); 
    glVertex2fv(PointE); 
    glVertex2fv(PointB); 
    glVertex2fv(PointD); 
    glEnd(); 
    glFlush(); 
}
/*画出正弦函数的图形 */
/* 由于OpenGL默认坐标值只能从-1到1，
（可以修改，但方法留到以后讲） 所以我们设置一个因子factor，
把所有的坐标值等比例缩小， 这样就可以画出更多个正弦周期 
试修改factor的值，观察变化情况 */
const GLfloat factor = 0.1f; 
void myDisplay3(void) 
{ 
    GLfloat x; 
    glClear(GL_COLOR_BUFFER_BIT); 
    glBegin(GL_LINES); 
    glVertex2f(-1.0f, 0.0f); 
    glVertex2f(1.0f, 0.0f); // 以上两个点可以画x轴 
    glVertex2f(0.0f, -1.0f); 
    glVertex2f(0.0f, 1.0f); // 以上两个点可以画y轴 
    glEnd(); 
    glBegin(GL_LINE_STRIP); 
    for(x=-1.0f/factor; x<1.0f/factor; x+=0.01f) 
    { 
        glVertex2f(x*factor, sin(x)*factor); 
    } 
    glEnd(); 
    glFlush(); 
}
int main(int argc, char *argv[]) 
{ 
    /*初始化*/
    glutInit(&argc, argv); 
    /*设置显示方式：RGB颜色，单缓冲*/
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE); 
    /*设置在窗口的中心位置*/
    glutInitWindowPosition(100, 100); 
    /*设置窗口大小*/
    glutInitWindowSize(200, 200); 
    /*创建窗口*/
    glutCreateWindow("Paint1"); 
    /*设置一个函数*/
    glutDisplayFunc(&myDisplay1); 
    /*创建窗口*/
    glutCreateWindow("Paint2"); 
    /*设置一个函数*/
    glutDisplayFunc(&myDisplay2); 
    /*创建窗口*/
    glutCreateWindow("Paint3"); 
    /*设置一个函数*/
    glutDisplayFunc(&myDisplay3); 
    /*进行一个消息循环*/
    glutMainLoop(); 
    return 0; 
}
```
***编译与运行：***
```
$ make
gcc test.c -lGL -lglut -lGLU -lXmu -Bstatic -Bdyanmic -lm
./a.out
```
***最终会跳出三个窗口，分别画出一个圆形，一个五角星和一个正弦函数：***
![](https://img-blog.csdn.net/20180505121003209)
代码以及内容来自***《OpenGL入门教程(精)》***，如有侵权，请联系删除。
