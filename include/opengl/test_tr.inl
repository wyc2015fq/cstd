#include <windows.h>
#include <stdio.h>
#include <gl/gl.h>
#include <gl/glut.h>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glut32.lib")
void init()
{
  glClearColor(0, 0, 0, 0);
  //开启顶点，颜色数组
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnable(GL_COLOR_ARRAY);
}
//用 glArrayElement方式绘制
void useGlArrayElement()
{
  static GLfloat vertexs[] = {
    -1, -1, 0,
    -0.5, -0.5, 0,
    -1, -0.5, 0
  };
  static GLfloat colors[] = {
    1, 0, 0,
    0, 1, 0,
    0, 0, 1
  };
  //绑定顶点数组
  glVertexPointer(3, GL_FLOAT, 0, vertexs);
  glColorPointer(3, GL_FLOAT, 0, colors);
  //解引用 和 绘制
  glBegin(GL_TRIANGLES);
  glArrayElement(1);
  glArrayElement(0);
  glArrayElement(2);
  glEnd();
}
void useGlDrawElements()
{
  static GLfloat vertexs[] = {
    -0.4, -1, 0,
    -0.2, -1, 0,
    -0.2, -0.5, 0
  };
  static GLfloat colors[] = {
    1, 0, 0,
    0, 1, 0,
    0, 0, 1
  };
  //绑定顶点数组
  glVertexPointer(3, GL_FLOAT, 0, vertexs);
  glColorPointer(3, GL_FLOAT, 0, colors);
  //解引用 和 绘制
  GLubyte array[] = {0, 1, 2};
  //type 必须是 无符号整形： GL_BYTE -- > gl
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, array);
}
//glDraw
void useGlDrawArrays()
{
  static GLfloat vertexs[] = {
    0, 0, 0,
    0, 0.5, 0,
    0.3, 0.4, 0
  };
  static GLfloat colors[] = {
    1, 0, 0,
    0, 1, 0,
    0, 0, 1
  };
  //绑定顶点数组
  glVertexPointer(3, GL_FLOAT, 0, vertexs);
  glColorPointer(3, GL_FLOAT, 0, colors);
  //gldraw
  glDrawArrays(GL_TRIANGLES, 0, 3);
}
void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  //gl
  useGlArrayElement();
  useGlDrawElements();
  useGlDrawArrays();
  glFlush();
}
//打印opengl信息
void printGlInfo()
{
  const GLubyte* name = glGetString(GL_VENDOR); //返回负责当前OpenGL实现厂商的名字
  const GLubyte* biaoshifu = glGetString(GL_RENDERER); //返回一个渲染器标识符，通常是个硬件平台
  const GLubyte* OpenGLVersion = glGetString(GL_VERSION); //返回当前OpenGL实现的版本号
  const GLubyte* gluVersion = gluGetString(GLU_VERSION); //返回当前GLU工具库版本
  printf("OpenGL实现厂商的名字：%s\n", name);
  printf("渲染器标识符：%s\n", biaoshifu);
  printf("OpenGL实现的版本号：%s\n", OpenGLVersion);
  printf("OGLU工具库版本：%s\n", gluVersion);
}
int test_tr(int argc, char* argv[])
{
  glutInit(&argc, (char**)argv);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("vertexArray");
  printGlInfo();
  init();
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}

