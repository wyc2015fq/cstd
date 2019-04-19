# OpenGL学习笔记：画点、直线和多边形（第二讲） - Koma Hub - CSDN博客
2018年05月05日 13:10:04[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：107
***1、关于点 ***
点的大小默认为1个像素，但也可以改变之。
改变的命令为glPointSize，其函数原型如下： 
void glPointSize(GLfloat size); 
size必须大于0.0f，默认值为1.0f，单位为“像素”。 
注意：对于具体的OpenGL实现，点的大小都有个限度的，如果设置的size超过最大值，则设置可能会有问题。 
例子：
```cpp
void myDisplay1(void) 
{ 
    glClear(GL_COLOR_BUFFER_BIT); 
    glPointSize(5.0f); 
    glBegin(GL_POINTS); 
    glVertex2f(0.0f, 0.0f); 
    glVertex2f(0.5f, 0.5f); 
    glEnd(); 
    glFlush(); 
}
```
***2、关于直线 ***
***（1）直线可以指定宽度：***
```cpp
void glLineWidth(GLfloat width);
```
其用法跟glPointSize类似。 
***（2）画虚线。 ***
首先，使用
```cpp
glEnable(GL_LINE_STIPPLE);
```
来启动虚线模式（使用glDisable(GL_LINE_STIPPLE)可以关闭之）。 
然后，使用glLineStipple来设置虚线的样式。 
```cpp
void glLineStipple(GLint factor, GLushort pattern);
```
pattern是由1和0组成的长度为16的序列，从最低位开始看，
如果为1，则直线上接下来应该画的factor个点将被画为实的；
如果为0，则直线上接下来应该画的factor个点将被画为虚的。 
以下是一些例子：
![](https://img-blog.csdn.net/20180505125708586)
声明：该图片来自www.opengl.org，该图片是《OpenGL编程指南》一书的附图，由于该书的旧版（第一版，1994年）已经流传于网络，我希望没有触及到版权问题。
示例代码：
```cpp
void myDisplay2(void) 
{ 
    glClear(GL_COLOR_BUFFER_BIT); 
    glEnable(GL_LINE_STIPPLE); 
    glLineStipple(2, 0x0F0F); 
    glLineWidth(10.0f); 
    glBegin(GL_LINES); 
    glVertex2f(0.0f, 0.0f); 
    glVertex2f(1.0f, 1.0f); 
    glEnd(); 
    glFlush(); 
}
```
***3、关于多边形 ***
多边形的内容较多，我们将讲述以下四个方面。 
***（1）多边形的两面以及绘制方式。***
虽然我们目前还没有真正的使用三维坐标来画图，但是建立一些三维的概念还是必要的。 
从三维的角度来看，一个多边形具有两个面。
每一个面都可以设置不同的绘制方式：填充、只绘制边缘轮廓线、只绘制顶点，其中“填充”是默认的方式。
可以为两个面分别设置不同的方式。
```cpp
glPolygonMode(GL_FRONT, GL_FILL); // 设置正面为填充方式 
glPolygonMode(GL_BACK, GL_LINE); // 设置反面为边缘绘制方式 
glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); // 设置两面均为顶点绘制方式
```
***（2）反转 ***
一般约定为“顶点以逆时针顺序出现在屏幕上的面”为“正面”，另一个面即成为“反面”。
生活中常见的物体表面，通常都可以用这样的“正面”和“反面”，“合理的”被表现出来（请找一个比较透明的矿泉水瓶子，在正对你的一面沿逆时针画一个圆，并标明画的方向，然后将背面转为正面，画一个类似的圆，体会一下“正面”和“反面”。你会发现正对你的方向，瓶的外侧是正面，而背对你的方向，瓶的内侧才是正面。正对你的内侧和背对你的外侧则是反面。这样一来，同样属于“瓶的外侧”这个表面，但某些地方算是正面，某些地方却算是反面了）。 
但也有一些表面比较特殊。例如“麦比乌斯带”（请自己Google一下），可以全部使用“正面”或全部使用“背面”来表示。
可以通过glFrontFace函数来交换“正面”和“反面”的概念。 
```cpp
glFrontFace(GL_CCW); // 设置CCW方向为“正面”，CCW即CounterClockWise，逆时针 
glFrontFace(GL_CW); // 设置CW方向为“正面”，CW即ClockWise，顺时针
```
下面是一个示例程序，请用它替换第一课中的myDisplay函数，并将glFrontFace(GL_CCW)修改为glFrontFace(GL_CW)，并观察结果的变化。
```cpp
void myDisplay3(void) 
{ 
    glClear(GL_COLOR_BUFFER_BIT); 
    glPolygonMode(GL_FRONT, GL_FILL); // 设置正面为填充模式 
    glPolygonMode(GL_BACK, GL_LINE); // 设置反面为线形模式 
    glFrontFace(GL_CCW); // 设置逆时针方向为正面 
    glBegin(GL_POLYGON); // 按逆时针绘制一个正方形，在左下方
    glVertex2f(-0.5f, -0.5f); 
    glVertex2f(0.0f, -0.5f); 
    glVertex2f(0.0f, 0.0f); 
    glVertex2f(-0.5f, 0.0f); 
    glEnd(); 
    glBegin(GL_POLYGON); // 按顺时针绘制一个正方形，在右上方 
    glVertex2f(0.0f, 0.0f); 
    glVertex2f(0.0f, 0.5f); 
    glVertex2f(0.5f, 0.5f); 
    glVertex2f(0.5f, 0.0f); 
    glEnd(); 
    glFlush(); 
}
```
***（3）剔除多边形表面 ***
在三维空间中，一个多边形虽然有两个面，但我们无法看见背面的那些多边形，而一些多边形虽然是正面的，但被其他多边形所遮挡。如果将无法看见的多边形和可见的多边形同等对待，无疑会降低我们处理图形的效率。在这种时候，可以将不必要的面剔除。 
首先，使用glEnable(GL_CULL_FACE);来启动剔除功能（使用glDisable(GL_CULL_FACE)可以关闭之） 
然后，使用glCullFace来进行剔除。 
glCullFace的参数可以是GL_FRONT，GL_BACK或者GL_FRONT_AND_BACK，分别表示剔除正面、剔除反面、剔除正反两面的多边形。 
注意：剔除功能只影响多边形，而对点和直线无影响。例如，使用glCullFace(GL_FRONT_AND_BACK)后，所有的多边形都将被剔除，所以看见的就只有点和直线。
***（4）镂空多边形 ***
直线可以被画成虚线，而多边形则可以进行镂空。 
首先，使用glEnable(GL_POLYGON_STIPPLE);来启动镂空模式（使用glDisable(GL_POLYGON_STIPPLE)可以关闭之）。 
然后，使用glPolygonStipple来设置镂空的样式。 
```cpp
void glPolygonStipple(const GLubyte *mask);
```
其中的参数mask指向一个长度为128字节的空间，它表示了一个32*32的矩形应该如何镂空。
其中：第一个字节表示了最左下方的从左到右（也可以是从右到左，这个可以修改）8个像素是否镂空（1表示不镂空，显示该像素；0表示镂空，显示其后面的颜色），最后一个字节表示了最右上方的8个像素是否镂空。 
但是，如果我们直接定义这个mask数组，像这样：
```cpp
static GLubyte Mask[128] = 
{ 0x00, 0x00, 0x00, 0x00, // 这是最下面的一行 
  0x00, 0x00, 0x00, 0x00, 
  0x03, 0x80, 0x01, 0xC0, // 麻 
  0x06, 0xC0, 0x03, 0x60, // 烦 
  0x04, 0x60, 0x06, 0x20, // 的 
  0x04, 0x30, 0x0C, 0x20, // 初 
  0x04, 0x18, 0x18, 0x20, // 始 
  0x04, 0x0C, 0x30, 0x20, // 化
  0x04, 0x06, 0x60, 0x20, // ， 
  0x44, 0x03, 0xC0, 0x22, // 不 
  0x44, 0x01, 0x80, 0x22, // 建 
  0x44, 0x01, 0x80, 0x22, // 议 
  0x44, 0x01, 0x80, 0x22, // 使 
  0x44, 0x01, 0x80, 0x22, // 用 
  0x44, 0x01, 0x80, 0x22, 
  0x44, 0x01, 0x80, 0x22, 
  0x66, 0x01, 0x80, 0x66, 
  0x33, 0x01, 0x80, 0xCC, 
  0x19, 0x81, 0x81, 0x98, 
  0x0C, 0xC1, 0x83, 0x30, 
  0x07, 0xE1, 0x87, 0xE0, 
  0x03, 0x3F, 0xFC, 0xC0, 
  0x03, 0x31, 0x8C, 0xC0, 
  0x03, 0x3F, 0xFC, 0xC0, 
  0x06, 0x64, 0x26, 0x60, 
  0x0C, 0xCC, 0x33, 0x30, 
  0x18, 0xCC, 0x33, 0x18, 
  0x10, 0xC4, 0x23, 0x08, 
  0x10, 0x63, 0xC6, 0x08,
  0x10, 0x30, 0x0C, 0x08, 
  0x10, 0x18, 0x18, 0x08, 
  0x10, 0x00, 0x00, 0x08 // 这是最上面的一行 
  };
```
**上面矩阵的图形会在后面给出bmp格式的单色位图图片。**
这样一堆数据非常缺乏直观性，我们需要很费劲的去分析，才会发现它表示的竟然是一只苍蝇。 
如果将这样的数据保存成图片，并用专门的工具进行编辑，显然会方便很多。
下面介绍如何做到这一点。
首先，用Windows自带的画笔程序新建一副图片，取名为mask.bmp，注意保存时，应该选择“**单色位图**”。
在“图象”->“属性”对话框中，设置图片的高度和宽度均为32。 
用放大镜观察图片，并编辑之。黑色对应二进制零（镂空），白色对应二进制一（不镂空），编辑完毕后保存。
然后，就可以使用以下代码来获得这个Mask数组了。 
```cpp
static GLubyte Mask[128];
FILE *fp; 
fp = fopen("mask.bmp", "rb"); 
if( !fp ) exit(0); 
// 移动文件指针到这个位置，使得再读sizeof(Mask)个字节就会遇到文件结束 
// 注意-(int)sizeof(Mask)虽然不是什么好的写法，但这里它确实是正确有效的 
// 如果直接写-sizeof(Mask)的话，因为sizeof取得的是一个无符号数，取负号会有问题 
if( fseek(fp, -(int)sizeof(Mask), SEEK_END) ) exit(0); 
// 读取sizeof(Mask)个字节到Mask 
if( !fread(Mask, sizeof(Mask), 1, fp) )
    exit(0); 
fclose(fp);
```
好的，现在请自己编辑一个图片作为mask，并用上述方法取得Mask数组，运行后观察效果。 
说明：绘制虚线时可以设置factor因子，但多边形的镂空无法设置factor因子。
请用鼠标改变窗口的大小，观察镂空效果的变化情况。
下面线给出两个bmp格式的文件**mask.bmp**和**mask1.bmp:**
![](https://img-blog.csdn.net/20180505130203119)![](https://img-blog.csdn.net/20180505130207286)
***运行环境：***
```
CentOS7
g++ (GCC) 4.8.5 20150623 (Red Hat 4.8.5-16)
OpenGL实现厂商的名字：VMware, Inc.
渲染器标识符：Gallium 0.4 on llvmpipe (LLVM 3.9, 256 bits)
OpenGL实现的版本号：2.1 Mesa 17.0.1
OGLU工具库版本：1.3
```
***贴上源代码：***
```cpp
/**
 * 《OpenGL入门教程》
 *               点、直线和多边形
 */
#include <GL/glut.h> 
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
/*画点*/
void myDisplay1(void) 
{ 
    glClear(GL_COLOR_BUFFER_BIT); 
    glPointSize(5.0f); 
    glBegin(GL_POINTS); 
    glVertex2f(0.0f, 0.0f); 
    glVertex2f(0.5f, 0.5f); 
    glEnd(); 
    glFlush(); 
}
/*画线段：实线、虚线。。。*/
void myDisplay2(void) 
{ 
    glClear(GL_COLOR_BUFFER_BIT); 
    glEnable(GL_LINE_STIPPLE); 
    /*line1*/
    glLineStipple(1, 0xAAAA); 
    glLineWidth(6.0f); 
    glBegin(GL_LINES); 
    glVertex2f(0.0f, 0.0f); 
    glVertex2f(1.0f, 1.0f); 
    glEnd(); 
    /*line2*/
    glLineStipple(4, 0xAAAA); 
    glLineWidth(6.0f); 
    glBegin(GL_LINES); 
    glVertex2f(0.0f, 0.0f); 
    glVertex2f(-1.0f, -1.0f); 
    glEnd(); 
    /*line3*/
    glLineStipple(2, 0x00FF); 
    glLineWidth(6.0f); 
    glBegin(GL_LINES); 
    glVertex2f(0.0f, 0.0f); 
    glVertex2f(-1.0f, 1.0f); 
    glEnd(); 
    /*line2*/
    glLineStipple(3, 0x0C0F); 
    glLineWidth(6.0f); 
    glBegin(GL_LINES); 
    glVertex2f(0.0f, 0.0f); 
    glVertex2f(1.0f, -1.0f); 
    glEnd(); 
    glFlush(); 
}
/*多边形*/
void myDisplay3(void) 
{ 
    glClear(GL_COLOR_BUFFER_BIT); 
    glPolygonMode(GL_FRONT, GL_FILL); // 设置正面为填充模式 
    glPolygonMode(GL_BACK, GL_LINE); // 设置反面为线形模式 
    glFrontFace(GL_CCW); // 设置逆时针方向为正面 
    //glFrontFace(GL_CW); // 设置逆时针方向为反面 
    glBegin(GL_POLYGON); // 按逆时针绘制一个正方形，在左下方
    glVertex2f(-0.5f, -0.5f); 
    glVertex2f(0.0f, -0.5f); 
    glVertex2f(0.0f, 0.0f); 
    glVertex2f(-0.5f, 0.0f); 
    glEnd(); 
    glBegin(GL_POLYGON); // 按顺时针绘制一个正方形，在右上方 
    glVertex2f(0.0f, 0.0f); 
    glVertex2f(0.0f, 0.5f); 
    glVertex2f(0.5f, 0.5f); 
    glVertex2f(0.5f, 0.0f); 
    glEnd(); 
    glFlush(); 
}
/*镂空多边形*/
static GLubyte Mask1[128] = 
{ 0x00, 0x00, 0x00, 0x00, // 这是最下面的一行 
  0x00, 0x00, 0x00, 0x00, 
  0x03, 0x80, 0x01, 0xC0, // 麻 
  0x06, 0xC0, 0x03, 0x60, // 烦 
  0x04, 0x60, 0x06, 0x20, // 的 
  0x04, 0x30, 0x0C, 0x20, // 初 
  0x04, 0x18, 0x18, 0x20, // 始 
  0x04, 0x0C, 0x30, 0x20, // 化
  0x04, 0x06, 0x60, 0x20, // ， 
  0x44, 0x03, 0xC0, 0x22, // 不 
  0x44, 0x01, 0x80, 0x22, // 建 
  0x44, 0x01, 0x80, 0x22, // 议 
  0x44, 0x01, 0x80, 0x22, // 使 
  0x44, 0x01, 0x80, 0x22, // 用 
  0x44, 0x01, 0x80, 0x22, 
  0x44, 0x01, 0x80, 0x22, 
  0x66, 0x01, 0x80, 0x66, 
  0x33, 0x01, 0x80, 0xCC, 
  0x19, 0x81, 0x81, 0x98, 
  0x0C, 0xC1, 0x83, 0x30, 
  0x07, 0xE1, 0x87, 0xE0, 
  0x03, 0x3F, 0xFC, 0xC0, 
  0x03, 0x31, 0x8C, 0xC0, 
  0x03, 0x3F, 0xFC, 0xC0, 
  0x06, 0x64, 0x26, 0x60, 
  0x0C, 0xCC, 0x33, 0x30, 
  0x18, 0xCC, 0x33, 0x18, 
  0x10, 0xC4, 0x23, 0x08, 
  0x10, 0x63, 0xC6, 0x08,
  0x10, 0x30, 0x0C, 0x08, 
  0x10, 0x18, 0x18, 0x08, 
  0x10, 0x00, 0x00, 0x08 // 这是最上面的一行 
  };
void myDisplay4(void) 
{ 
    static GLubyte Mask[128]; 
    FILE *fp; 
    fp = fopen("mask.bmp", "rb"); 
    if( !fp ) exit(0); 
    if( fseek(fp, -(int)sizeof(Mask), SEEK_END) ) 
        exit(0); 
    if( !fread(Mask, sizeof(Mask), 1, fp) ) 
        exit(0); 
    fclose(fp); 
    glClear(GL_COLOR_BUFFER_BIT); 
    glEnable(GL_POLYGON_STIPPLE); 
    glPolygonStipple(Mask1); //可以给成读取图片mask.bmp的Mask
    glRectf(-1.0f, -1.0f, 0.0f, 0.0f); // 在左下方绘制一个有镂空效果的正方形 
    glDisable(GL_POLYGON_STIPPLE); 
    glRectf(0.0f, 0.0f, 1.0f, 1.0f); // 在右上方绘制一个无镂空效果的正方形 
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
    /*创建窗口*/
    glutCreateWindow("Paint4"); 
    /*设置一个函数*/
    glutDisplayFunc(&myDisplay4); 
    /*进行一个消息循环*/
    glutMainLoop(); 
    return 0; 
}
```
***编译和运行：***
```
$ make
gcc test.c -lGL -lglut -lGLU -lXmu -Bstatic -Bdyanmic -lm
./a.out
```
运行后会跳出四个窗口，分别画出***点、线、多边形和镂空多边形：***
![](https://img-blog.csdn.net/2018050513045812)
代码以及内容来自《**OpenGL入门教程(精)**》，如有侵权，请联系删除。
