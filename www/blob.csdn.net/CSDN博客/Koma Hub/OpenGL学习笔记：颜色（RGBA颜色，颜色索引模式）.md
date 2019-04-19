# OpenGL学习笔记：颜色（RGBA颜色，颜色索引模式） - Koma Hub - CSDN博客
2018年05月05日 14:38:45[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：1101
OpenGL支持**两种颜色模式**：一种是**RGBA**，一种是颜色**索引模式**。 
无论哪种颜色模式，计算机都必须为每一个像素保存一些数据。
不同的是，RGBA模式中，数据直接就代表了颜色；而颜色索引模式中，数据代表的是一个索引，要得到真正的颜色，还必须去查索引表。
***1. RGBA颜色***
RGBA模式中，每一个像素会保存以下数据：R值（红色分量）、G值（绿色分量）、B值（蓝色分量）和A值（alpha分量）。其中红、绿、蓝三种颜色相组合，就可以得到我们所需要的各种颜色，而alpha不直接影响颜色，它将留待以后介绍。
 在RGBA模式下选择颜色是十分简单的事情，只需要一个函数就可以搞定。 
glColor*系列函数可以用于设置颜色，其中三个参数的版本可以指定R、G、B的值，而A值采用默认；
四个参数的版本可以分别指定R、G、B、A的值。
例如：
```cpp
void glColor3f(GLfloat red, GLfloat green, GLfloat blue); 
void glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
```
（3f表示有三个浮点参数~请看第二课中关于glVertex*函数的叙述。） 
将浮点数作为参数，其中0.0表示不使用该种颜色，而1.0表示将该种颜色用到最多。
例如：
```cpp
glColor3f(1.0f, 0.0f, 0.0f); //表示不使用绿、蓝色，而将红色使用最多，于是得到最纯净的红色。 
glColor3f(0.0f, 1.0f, 1.0f); //表示使用绿、蓝色到最多，而不使用红色。混合的效果就是浅蓝色。 
glColor3f(0.5f, 0.5f, 0.5f); //表示各种颜色使用一半，效果为灰色。
```
注意：浮点数可以精确到小数点后若干位，这并不表示计算机就可以显示如此多种颜色。
实际上，计算机可以显示的颜色种数将由硬件决定。如果OpenGL找不到精确的颜色，会进行类似“四舍五入”的处理。
大家可以通过改变下面代码中glColor3f的参数值，绘制不同颜色的矩形。
```cpp
void myDisplay1(void) 
{ 
    glClear(GL_COLOR_BUFFER_BIT); 
    glColor3f(0.0f, 1.0f, 1.0f); 
    glRectf(-0.5f, -0.5f, 0.0f, 0.0f); 
    glFlush(); 
}
```
注意：glColor系列函数，在参数类型不同时，表示“最大”颜色的值也不同。 
采用f和d做后缀的函数，以1.0表示最大的使用。 
采用b做后缀的函数，以127表示最大的使用。 
采用ub做后缀的函数，以255表示最大的使用。 
采用s做后缀的函数，以32767表示最大的使用。 
采用us做后缀的函数，以65535表示最大的使用。 
这些规则看似麻烦，但熟悉后实际使用中不会有什么障碍。
***2、索引颜色 ***
在索引颜色模式中，OpenGL需要一个颜色表。这个表就相当于画家的调色板：虽然可以调出很多种颜色，但同时存在于调色板上的颜色种数将不会超过调色板的格数。试将颜色表的每一项想象成调色板上的一个格子：它保存了一种颜色。 
在使用索引颜色模式画图时，我说“我把第i种颜色设置为某某”，其实就相当于将调色板的第i格调为某某颜色。“我需要第k种颜色来画图”，那么就用画笔去蘸一下第k格调色板。 颜色表的大小是很有限的，一般在256~4096之间，且总是2的整数次幂。在使用索引颜色方式进行绘图时，总是先设置颜色表，然后选择颜色。
***2.1、选择颜色 ***
使用glIndex*系列函数可以在颜色表中选择颜色。其中最常用的可能是glIndexi，它的参数是一个整形。 
void glIndexi(GLint c);
***2.2、设置颜色表***
OpenGL并直接没有提供设置颜色表的方法，因此设置颜色表需要使用操作系统的支持。我们所用的Windows和其他大多数图形操作系统都具有这个功能，但所使用的函数却不相同。正如我没有讲述如何自己写代码在Windows下建立一个窗口，这里我也不会讲述如何在Windows下设置颜色表。 GLUT工具包提供了设置颜色表的函数glutSetColor，但我测试始终有问题。现在为了让大家体验一下索引颜色，我向大家介绍另一个OpenGL工具包：aux。这个工具包是VisualStudio自带的，不必另外安装，但它已经过时，这里仅仅是体验一下，大家不必深入。
```cpp
/* glaux.h未安装，以后做讨论
#include <GL/glaux.h>
#pragma comment (lib, "opengl32.lib") 
#pragma comment (lib, "glaux.lib")
const GLdouble Pi = 3.1415926536; 
void myDisplay5(void) 
{ 
    int i; 
    for(i=0; i<8; ++i) 
        auxSetOneColor(i, (float)(i&0x04), (float)(i&0x02), (float)(i&0x01)); 
    glShadeModel(GL_FLAT); 
    glClear(GL_COLOR_BUFFER_BIT); 
    glBegin(GL_TRIANGLE_FAN); 
    glVertex2f(0.0f, 0.0f); 
    for(i=0; i<=8; ++i) 
    { 
        glIndexi(i); 
        glVertex2f(cos(i*Pi/4), sin(i*Pi/4)); 
    } 
    glEnd(); 
    glFlush(); 
}*/
```
其它部分大家都可以不管，只看myDisplay函数就可以了。
首先，使用auxSetOneColor设置颜色表中的一格。循环八次就可以设置八格。 glShadeModel等下再讲，这里不提。 
然后在循环中用glVertex设置顶点，同时用glIndexi改变顶点代表的颜色。 
最终得到的效果是八个相同形状、不同颜色的三角形。 索引颜色虽然讲得多了点。
索引颜色的主要优势是占用空间小（每个像素不必单独保存自己的颜色，只用很少的二进制位就可以代表其颜色在颜色表中的位置），花费系统资源少，图形运算速度快，但它编程稍稍显得不是那么方便，并且画面效果也会比RGB颜色差一些。“星际争霸”可能代表了256色的颜色表的画面效果，虽然它在一台很烂的PC上也可以运行很流畅，但以目前的眼光来看，其画面效果就显得不足了。 目前的PC机性能已经足够在各种场合下使用RGB颜色，因此PC程序开发中，使用索引颜色已经不是主流。当然，一些小型设备例如GBA、手机等，索引颜色还是有它的用武之地。
***3、指定清除屏幕用的颜色 ***
glClear(GL_COLOR_BUFFER_BIT);
意思是把屏幕上的颜色清空。 
但实际上什么才叫“空”呢？在宇宙中，黑色代表了“空”；在一张白纸上，白色代表了“空”；在信封上，信封的颜色才是“空”。 
OpenGL用下面的函数来定义清楚屏幕后屏幕所拥有的颜色。 
在RGB模式下，使用glClearColor来指定“空”的颜色，它需要四个参数，其参数的意义跟glColor4f相似。 
在索引颜色模式下，使用glClearIndex来指定“空”的颜色所在的索引，它需要一个参数，其意义跟glIndexi相似。
```cpp
/*指定清除屏幕用的颜色*/
void myDisplay3(void) 
{ 
    glClearColor(1.0f, 1.0f, 0.0f, 0.0f); 
    glClear(GL_COLOR_BUFFER_BIT); 
    glFlush(); 
}
```
***4、指定着色模型 ***
OpenGL允许为同一多边形的不同顶点指定不同的颜色。例如：
```cpp
/*指定着色模型*/
const GLdouble Pi = 3.1415926536; 
void myDisplay4(void) 
{ 
    int i; 
    // glShadeModel(GL_FLAT); 
    glClear(GL_COLOR_BUFFER_BIT); 
    glBegin(GL_TRIANGLE_FAN); 
    glColor3f(1.0f, 1.0f, 1.0f); 
    glVertex2f(0.0f, 0.0f); 
    for(i=0; i<=8; ++i) 
    { 
        glColor3f(i&0x04, i&0x02, i&0x01);
        glVertex2f(cos(i*Pi/4), sin(i*Pi/4)); 
    } 
    glEnd(); 
    glFlush(); 
}
```
在默认情况下，OpenGL会计算两点顶点之间的其它点，并为它们填上“合适”的颜色，使相邻的点的颜色值都比较接近。如果使用的是RGB模式，看起来就具有渐变的效果。如果是使用颜色索引模式，则其相邻点的索引值是接近的，如果将颜色表中接近的项设置成接近的颜色，则看起来也是渐变的效果。但如果颜色表中接近的项颜色却差距很大，则看起来可能是很奇怪的效果。 
使用glShadeModel函数可以关闭这种计算，如果顶点的颜色不同，则将顶点之间的其它点全部设置为与某一个点相同。（直线以后指定的点的颜色为准，而多边形将以任意顶点的颜色为准，由实现决定。）为了避免这个不确定性，尽量在多边形中使用同一种颜色。 
**glShadeModel的使用方法**： 
```cpp
glShadeModel(GL_SMOOTH); // 平滑方式，这也是默认方式 
glShadeModel(GL_FLAT); // 单色方式
```
***小结：***
本课学习了如何设置颜色。其中RGB颜色方式是目前PC机上的常用方式。 
可以设置glClear清除后屏幕所剩的颜色。 
可以设置颜色填充方式：平滑方式或单色方式。
***运行环境：***
```
CentOS7
g++ (GCC) 4.8.5 20150623 (Red Hat 4.8.5-16)
OpenGL实现厂商的名字：VMware, Inc.
渲染器标识符：Gallium 0.4 on llvmpipe (LLVM 3.9, 256 bits)
OpenGL实现的版本号：2.1 Mesa 17.0.1
OGLU工具库版本：1.3
```
下面给出***源代码：***
```cpp
/**
 * 《OpenGL入门教程》
 *         RGBA颜色，颜色索引模式。
 */
#include <GL/gl.h> 
#include <GL/glu.h> 
#include <GL/glut.h> 
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
/*RGBA颜色*/
void myDisplay1(void) 
{ 
    glClear(GL_COLOR_BUFFER_BIT); 
    glColor3f(0.0f, 1.0f, 1.0f); 
    glRectf(-0.5f, -0.5f, 0.0f, 0.0f); 
    glColor3f(0.0f, 1.0f, 0.0f); 
    glRectf(-0.5f, -0.5f, 0.0f, 0.0f); 
    glFlush(); 
}
/*RGBA色水壶轮廓*/
void myDisplay2(void)
{
    glClearColor(1.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-5, 5, -5, 5, 5, 15);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 0);
    glutWireTeapot(3);
    glFlush();
}
/*指定清除屏幕用的颜色*/
void myDisplay3(void) 
{ 
    glClearColor(1.0f, 1.0f, 0.0f, 0.0f); 
    glClear(GL_COLOR_BUFFER_BIT); 
    glFlush(); 
}
/*指定着色模型*/
const GLdouble Pi = 3.1415926536; 
void myDisplay4(void) 
{ 
    int i; 
    // glShadeModel(GL_FLAT); 
    glClear(GL_COLOR_BUFFER_BIT); 
    glBegin(GL_TRIANGLE_FAN); 
    glColor3f(1.0f, 1.0f, 1.0f); 
    glVertex2f(0.0f, 0.0f); 
    for(i=0; i<=8; ++i) 
    { 
        glColor3f(i&0x04, i&0x02, i&0x01);
        glVertex2f(cos(i*Pi/4), sin(i*Pi/4)); 
    } 
    glEnd(); 
    glFlush(); 
}
/* glaux.h未安装，以后做讨论
#include <GL/glaux.h>
#pragma comment (lib, "opengl32.lib") 
#pragma comment (lib, "glaux.lib")
const GLdouble Pi = 3.1415926536; 
void myDisplay5(void) 
{ 
    int i; 
    for(i=0; i<8; ++i) 
        auxSetOneColor(i, (float)(i&0x04), (float)(i&0x02), (float)(i&0x01)); 
    glShadeModel(GL_FLAT); 
    glClear(GL_COLOR_BUFFER_BIT); 
    glBegin(GL_TRIANGLE_FAN); 
    glVertex2f(0.0f, 0.0f); 
    for(i=0; i<=8; ++i) 
    { 
        glIndexi(i); 
        glVertex2f(cos(i*Pi/4), sin(i*Pi/4)); 
    } 
    glEnd(); 
    glFlush(); 
}*/
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
***编译与运行：***
```
$ make
gcc test.c -lGL -lglut -lGLU -lXmu -Bstatic -Bdyanmic -lm
./a.out
```
***结果图：***
![](https://img-blog.csdn.net/20180505143553904)
**代码以及内容来自《*OpenGL入门教程(精)*》，如有侵权，请联系删除。**
**最后给一个Terrence Ma写的代码**：
```cpp
// Name     : OpenGL Color Cube
// Author   : Terrence Ma
// Email    : terrence@terrence.com
// Web      : http://www.terrence.com
// Date     : 10/25/2001
// Modified : Tutorial sample from Mesa3d.org (http://www.mesa3d.org)
 
/*
 * Copyright (c) 1993-1997, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED 
 * Permission to use, copy, modify, and distribute this software for 
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that 
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. 
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * US Government Users Restricted Rights 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
 */
 
/*
 *  aapoly.c
 *  This program draws filled polygons with antialiased
 *  edges.  The special GL_SRC_ALPHA_SATURATE blending 
 *  function is used.
 *  Pressing the 't' key turns the antialiasing on and off.
 */
#include <GL/glut.h>
#include <stdlib.h>
 
GLboolean polySmooth = GL_TRUE;
 
static void init(void)
{
    glCullFace (GL_BACK);
    glEnable (GL_CULL_FACE);
    glBlendFunc (GL_SRC_ALPHA_SATURATE, GL_ONE);
    glClearColor (0.0, 0.0, 0.0, 0.0);
}
 
#define NFACE 6
#define NVERT 8
void drawCube(GLdouble x0, GLdouble x1, GLdouble y0, GLdouble y1,
        GLdouble z0, GLdouble z1)
{
   static GLfloat v[8][3];
   static GLfloat c[8][4] = {
      {0.0, 0.0, 0.0, 1.0}, {1.0, 0.0, 0.0, 1.0},
      {0.0, 1.0, 0.0, 1.0}, {1.0, 1.0, 0.0, 1.0},
      {0.0, 0.0, 1.0, 1.0}, {1.0, 0.0, 1.0, 1.0},
      {0.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}
   };
 
/*  indices of front, top, left, bottom, right, back faces  */
   static GLubyte indices[NFACE][4] = {
      {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3},
      {0, 1, 5, 4}, {1, 5, 6, 2}, {0, 3, 2, 1}
   };
 
   v[0][0] = v[3][0] = v[4][0] = v[7][0] = x0;
   v[1][0] = v[2][0] = v[5][0] = v[6][0] = x1;
   v[0][1] = v[1][1] = v[4][1] = v[5][1] = y0;
   v[2][1] = v[3][1] = v[6][1] = v[7][1] = y1;
   v[0][2] = v[1][2] = v[2][2] = v[3][2] = z0;
   v[4][2] = v[5][2] = v[6][2] = v[7][2] = z1;
 
#ifdef GL_VERSION_1_1
   glEnableClientState (GL_VERTEX_ARRAY);
   glEnableClientState (GL_COLOR_ARRAY);
   glVertexPointer (3, GL_FLOAT, 0, v);
   glColorPointer (4, GL_FLOAT, 0, c);
   glDrawElements (GL_QUADS, NFACE*4, GL_UNSIGNED_BYTE, indices);
   glDisableClientState (GL_VERTEX_ARRAY);
   glDisableClientState (GL_COLOR_ARRAY);
#else
   printf ("If this is GL Version 1.0, ");
   printf ("vertex arrays are not supported.\n");
   exit(1);
#endif
}
 
/*  Note:  polygons must be drawn from front to back
 *  for proper blending.
 */
void display(void)
{
   if (polySmooth) {
      glClear (GL_COLOR_BUFFER_BIT);
      glEnable (GL_BLEND);
      glEnable (GL_POLYGON_SMOOTH);
      glDisable (GL_DEPTH_TEST);
   }
   else { 
      glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glDisable (GL_BLEND);
      glDisable (GL_POLYGON_SMOOTH);
      glEnable (GL_DEPTH_TEST);
   }
 
   glPushMatrix ();
      glTranslatef (0.0, 0.0, -8.0);    
      glRotatef (30.0, 1.0, 0.0, 0.0);
      glRotatef (60.0, 0.0, 1.0, 0.0); 
      drawCube(-0.8, 0.8, -0.8, 0.8, -0.8, 0.8);
   glPopMatrix ();
 
   glFlush ();
}
 
void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(30.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}
 
/* ARGSUSED1 */
void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 'T':
      case 't':
         polySmooth = !polySmooth;
         glutPostRedisplay();
         break;
      case 27:
         exit(0);  /*  Escape key  */
         break;
      default:
         break;
   }
}
 
/*  Main Loop
 */
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB 
                        | GLUT_ALPHA | GLUT_DEPTH);
   glutInitWindowSize(200, 200);
   glutCreateWindow("OpenGL Color Cube");
   init ();
   glutReshapeFunc (reshape);
   glutKeyboardFunc (keyboard);
   glutDisplayFunc (display);
   glutMainLoop();
   return 0;
}
```
编译方法同上，结果为：
![](https://img-blog.csdn.net/20180505144656592)
