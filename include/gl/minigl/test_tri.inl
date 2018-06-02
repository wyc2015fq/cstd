
// OpenGL编程轻松入门之一个简单的例子
// 先编译运行一个简单的例子，这样我们可以有一个直观的印象。从这个例子我们可以看到OpenGL可以做什么，当然这个例子只做了很简单的一件事--绘制一个彩色的三角形。除此以外，我们还可以看到典型的OpenGL程序结构及openGL的运行顺序。

// 例1：本例在黑色的背景下绘制一个彩色的三角形，如图一所示。

//static GLuint Object;

#include <gl/gl.h>
#include <gl/glut.h>

#if 0
static GLuint make_object(void)
{
  //GLuint list;

  //list = glGenLists(1);

  //glNewList(list, GL_COMPILE);

  glBegin(GL_TRIANGLES);//开始画三角形
  glShadeModel(GL_SMOOTH);//设置为光滑明暗模式

  glColor3f(1.0, 0.0, 0.0); //设置第一个顶点为红色
  glVertex2f(-1.0, -1.0); //设置第一个顶点的坐标为（-1.0，-1.0）

  glColor3f(0.0, 1.0, 0.0); //设置第二个顶点为绿色
  glVertex2f(0.0, -1.0); //设置第二个顶点的坐标为（0.0，-1.0）

  glColor3f(0.0, 0.0, 1.0); //设置第三个顶点为蓝色
  glVertex2f(-0.5, 1.0); //设置第三个顶点的坐标为（-0.5，1.0）
  glEnd();//三角形结束

  //glEndList();

  return list;
}
#endif


void init(void)
{
  //Object = make_object();
  //glCullFace(GL_BACK);
  /*   glEnable( GL_CULL_FACE );*/
  
  //glDisable(GL_DITHER);
  //glShadeModel(GL_SMOOTH);

  glClearColor(0.0, 0.0, 0.0, 0.0); //设置背景颜色为黑色
}

void draw(void)
{
  glClear(GL_COLOR_BUFFER_BIT);//buffer设置为颜色可写
  //glCallList(Object);
  glBegin(GL_TRIANGLES);//开始画三角形
  glShadeModel(GL_SMOOTH);//设置为光滑明暗模式
  
  glColor3f(1.0, 0.0, 0.0); //设置第一个顶点为红色
  glVertex2f(-1.0, -1.0); //设置第一个顶点的坐标为（-1.0，-1.0）
  
  glColor3f(0.0, 1.0, 0.0); //设置第二个顶点为绿色
  glVertex2f(0.0, -1.0); //设置第二个顶点的坐标为（0.0，-1.0）
  
  glColor3f(0.0, 0.0, 1.0); //设置第三个顶点为蓝色
  glVertex2f(-0.5, 1.0); //设置第三个顶点的坐标为（-0.5，1.0）
  glEnd();//三角形结束
  
  glFlush();//强制OpenGL函数在有限时间内运行
  //glutSwapBuffers();
}

void idle(void)
{
  draw();
}

void reshape(int width, int height)
{
  GLfloat  h = (GLfloat) height / (GLfloat) width;

  glViewport(0, 0, (GLint)width, (GLint)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
#if 0
  glFrustum(-1.0, 1.0, -h, h, 5.0, 60.0);
#else
  glOrtho(-1.0, 1.0, -1, 1, 5.0, 60.0);
#endif
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -10.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLenum key(int k, GLenum mask)
{
  switch (k) {
  case GLUT_KEY_F1:
    exit(0);
  }

  return GL_FALSE;
}

int test_tri(int argc, char** argv)
{
  /*初始化*/
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(400, 400);
  glutInitWindowPosition(200, 200);

  /*创建窗口*/
  glutCreateWindow("Triangle");

  /*绘制与显示*/
  //background();
  glutReshapeFunc(reshape);
  glutDisplayFunc(draw);

  glutMainLoop();
  return(0);
}


#if 0
图一：一个彩色的三角形

首先创建工程，其步骤如下：

1）创建一个Win32 Console Application。

2）链接OpenGL libraries。在Visual C++中先单击Project，再单击Settings，再找到Link单击，最后在Object / library modules 的最前面加上OpenGL32.lib GLu32.lib GLaux.lib

3）单击Project Settings中的C / C++标签，将Preprocessor definitions 中的_CONSOLE改为__WINDOWS。最后单击OK。

现在你可以把下面的例子拷贝到工程中去，编译运行。你可以看到一个彩色的三角形。

我们先看看main函数。函数中以glut开头的函数都包含在glut.h中。GLUT库的函数主要执行如处理多窗口绘制、处理回调驱动事件、生成层叠式弹出菜单、绘制位图字体和笔画字体，以及各种窗口管理等任务。

glutInit用来初始化GLUT库并同窗口系统对话协商。

glutInitDisplayMode用来确定所创建窗口的显示模式。本例中的参数GLUT_SINGLE 指定单缓存窗口，这也是缺省模式，对应的模式为GLUT_DOUBLE 双缓存窗口。参数GLUT_RGB指定颜色RGBA模式，这也是缺省模式，对应的模式为GLUT_INDEX 颜色索引模式窗口。

glutInitWindowSize 初始化窗口的大小，第一个参数为窗口的宽度，第二个参数为窗口的高度，以像素为单位。

glutInitWindowPosition 设置初始窗口的位置，第一个参数为窗口左上角x的坐标，第二个参数为窗口左上角y的坐标，以像素为单位。屏幕的左上角的坐标为（0，0），横坐标向右逐渐增加，纵坐标向下逐渐增加。

glutCreateWindow 创建顶层窗口，窗口的名字为扩号中的参数。

background() 这是自己写的函数，设置背景。其实这个函数中的语句可以写在display函数中，但为了使功能块更加清晰，所以把背景这一部分单独提出来。

glutReshapeFunc 注册当前窗口的形状变化回调函数。当改变窗口大小时，该窗口的形状改变回调函数将被调用。在此例中就是myReshape指定形状变化函数。

glutDisplayFunc 注册当前窗口的显示回调函数。当一个窗口的图像层需要重新绘制时，GLUT将调用该窗口的的显示回调函数。在此例中的mydisplay就是显示回调函数，显示回调函数不带任何参数，它负责整个图像层的绘制。我们的大部分工作将集中在这个函数中。

glutMainLoop 进入GLUT事件处理循环。glutMainLoop函数在GLUT程序中最多只能调用一次，它一旦被调用就不再返回，并且调用注册过的回调函数。所以这个函数必须放在注册回调函数的后面，此例中为glutReshapeFunc， glutDisplayFunc。


#endif
