#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include "opengl_utilities.inl"
GLint WinW = 400;
GLint WinH = 400;
GLint dx, dy;
GLfloat xsize, ysize, zsize;
GLfloat xangle, yangle;
GLfloat oldx, oldy;
GLfloat position[] = { 1.0, 1.0, -1.0, 1.0};
void init()
{
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glShadeModel(GL_SMOOTH);
  dx = 0;
  dy = 0;
  xangle = 0;
  yangle = 0;
  xsize = 1.0;
  ysize = 1.0;
  zsize = 1.0;
  glEnable(GL_DEPTH_TEST);
  glLightfv(GL_LIGHT0, GL_POSITION, position);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
}
void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();
  glRotatef(xangle, 1.0, 0.0, 0.0);
  glRotatef(yangle, 0.0, 1.0, 0.0);
  glScalef(xsize, ysize, zsize);
  glutSolidTeapot(0.4);
  glPopMatrix();
  glutSwapBuffers();
}
void mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    oldx = x;//当左键按下时记录鼠标坐标
    oldy = y;
  }
}
void motion(int x, int y)
{
  GLint deltax = oldx - x;
  GLint deltay = oldy - y;
  yangle += 360 * (GLfloat)deltax / (GLfloat)WinW; //根据屏幕上鼠标滑动的距离来设置旋转的角度
  xangle += 360 * (GLfloat)deltay / (GLfloat)WinH;
  oldx = x;//记录此时的鼠标坐标，更新鼠标坐标
  oldy = y;//若是没有这两句语句，滑动是旋转会变得不可控
  glutPostRedisplay();
}
void reshape(int w, int h)
{
  WinW = w;
  WinH = h;
  glViewport(0.0, 0.0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (1) {
    glFrustum(-1, 1, -1.0, 1.0, -1.0, 1.0);
  }
  else {
    GLfloat ratio = (GLfloat)w / (GLfloat)h;
    if (w <= h) { //保持形状不变
      gluOrtho2D(-1.0, 1.0, -1.0 / ratio, 1.0 / ratio);
    }
    else {
      gluOrtho2D(-1.0 * ratio, 1.0 * ratio, -1.0, 1.0);
    }
  }
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
int test_trackball(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(WinW, WinH);
  glutCreateWindow("Test");
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutMainLoop();
}

