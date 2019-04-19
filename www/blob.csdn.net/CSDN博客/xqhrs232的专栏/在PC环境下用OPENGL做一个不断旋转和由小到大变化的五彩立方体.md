# 在PC环境下用OPENGL做一个不断旋转和由小到大变化的五彩立方体 - xqhrs232的专栏 - CSDN博客
2009年11月20日 11:19:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1184
 //#include "glos.h"
#include<windows.h>//这个必须包含，要不然会有很多H文件报错出来！！！
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
/* 定义旋转和比例因子的初始值 */
GLfloat step=0.0,s=0.1;
/* 定义立方体的顶点坐标值 */
static GLfloat  p1[]={0.5,-0.5,-0.5}, p2[]={0.5,0.5,-0.5},
                p3[]={0.5,0.5,0.5},   p4[]={0.5,-0.5,0.5},
                p5[]={-0.5,-0.5,0.5}, p6[]={-0.5,0.5,0.5},
                p7[]={-0.5,0.5,-0.5}, p8[]={-0.5,-0.5,-0.5};
/* 定义立方体的顶点方向值 */
static GLfloat  m1[]={1.0,0.0,0.0}, m2[]={-1.0,0.0,0.0},
                m3[]={0.0,1.0,0.0}, m4[]={0.0,-1.0,0.0},
                m5[]={0.0,0.0,1.0}, m6[]={0.0,0.0,-1.0};
/* 定义立方体的顶点颜色值 */
static GLfloat  c1[]={0.0,0.0,1.0},  c2[]={0.0,1.0,1.0},
                c3[]={1.0,1.0,1.0},  c4[]={1.0,0.0,1.0},
                c5[]={1.0,0.0,0.0},  c6[]={1.0,1.0,0.0},
                c7[]={0.0,1.0,0.0},  c8[]={1.0,1.0,1.0};
void myinit(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK stepDisplay (void);
void CALLBACK startIdleFunc (AUX_EVENTREC *event);
void CALLBACK stopIdleFunc (AUX_EVENTREC *event);
void CALLBACK display(void);
void DrawColorBox(void);
void myinit(void)
{
    GLfloat light_ambient[]={0.3,0.2,0.5};
    GLfloat light_diffuse[]={1.0,1.0,1.0};
    GLfloat light_position[] = { 2.0, 2.0, 2.0, 1.0 };
    GLfloat light1_ambient[]={0.3,0.3,0.2};
    GLfloat light1_diffuse[]={1.0,1.0,1.0};
    GLfloat light1_position[] = { -2.0, -2.0, -2.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
     glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE);
     glEnable(GL_COLOR_MATERIAL);
}
void CALLBACK display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      s+=0.005;
      if(s>1.0)s=0.1;
    glPushMatrix();
     glScalef(s,s,s);
     glRotatef(step,0.0,1.0,0.0);
     glRotatef(step,0.0,0.0,1.0);
     glRotatef(step,1.0,0.0,0.0);
     DrawColorBox();
    glPopMatrix();
    glFlush();
    auxSwapBuffers();     /*  交换缓存  */
}
void CALLBACK stepDisplay (void)
{
    step = step + 1.0;
    if (step > 360.0)
    step = step - 360.0;
    display();
}
void CALLBACK startIdleFunc (AUX_EVENTREC *event)
{
    auxIdleFunc(stepDisplay);
}
void CALLBACK stopIdleFunc (AUX_EVENTREC *event)
{
    auxIdleFunc(0);
}
void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
    glOrtho (-1.5, 1.5, -1.5*(GLfloat)h/(GLfloat)w,
        1.50*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
    else
    glOrtho (-1.5*(GLfloat)w/(GLfloat)h,
        1.5*(GLfloat)w/(GLfloat)h, -1.5, 1.5, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();
}
  void DrawColorBox(void)
{
   glBegin (GL_QUADS);
         glColor3fv(c1);
         glNormal3fv(m1);
        glVertex3fv(p1);
        glColor3fv(c2);
        glVertex3fv(p2);
        glColor3fv(c3);
        glVertex3fv(p3);
        glColor3fv(c4);
        glVertex3fv(p4);
         glColor3fv(c5);
         glNormal3fv(m5);
        glVertex3fv(p5);
        glColor3fv(c6);
        glVertex3fv(p6);
        glColor3fv(c7);
        glVertex3fv(p7);
        glColor3fv(c8);
        glVertex3fv(p8);
         glColor3fv(c5);
         glNormal3fv(m3);
        glVertex3fv(p5);
        glColor3fv(c6);
        glVertex3fv(p6);
        glColor3fv(c3);
        glVertex3fv(p3);
        glColor3fv(c4);
        glVertex3fv(p4);
         glColor3fv(c1);
         glNormal3fv(m4);
        glVertex3fv(p1);
        glColor3fv(c2);
        glVertex3fv(p2);
        glColor3fv(c7);
        glVertex3fv(p7);
        glColor3fv(c8);
        glVertex3fv(p8);
         glColor3fv(c2);
         glNormal3fv(m5);
        glVertex3fv(p2);
        glColor3fv(c3);
        glVertex3fv(p3);
        glColor3fv(c6);
        glVertex3fv(p6);
        glColor3fv(c7);
        glVertex3fv(p7);
         glColor3fv(c1);
         glNormal3fv(m6);
        glVertex3fv(p1);
        glColor3fv(c4);
        glVertex3fv(p4);
        glColor3fv(c5);
        glVertex3fv(p5);
        glColor3fv(c8);
        glVertex3fv(p8);
    glEnd();
}
void main(void)
{
    auxInitDisplayMode (AUX_DOUBLE | AUX_RGBA); /* 设置双缓存和RGBA颜色模式*/
    auxInitPosition (0, 0, 500,400);
    auxInitWindow ("Double_Buffer_Animation");
    myinit();
    auxReshapeFunc (myReshape);
    auxIdleFunc (stepDisplay);
    auxMouseFunc (AUX_LEFTBUTTON, AUX_MOUSEDOWN, startIdleFunc);
    auxMouseFunc (AUX_RIGHTBUTTON, AUX_MOUSEDOWN, stopIdleFunc);
    auxMainLoop(display);
}
以上程序运行结果是在屏幕上显示一个不断旋转和由小到大变化的五彩立方体。
在PC环境下VS2005 IDE下建立工程----------//glut库！！！
     //1111-----H头文件-----gl.h++++glu.h++++glaux.h
     //2222-----LIB库文件----glu32.lib++++glaux.lib++++OpenGL32.Lib
     glu32.lib  glaux.lib  OpenGL32.Lib
     //3333-----dll库文件----glmf32.dll++++glu32.dll++++opengl32.dll
