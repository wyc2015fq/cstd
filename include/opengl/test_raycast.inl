#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <io.h>
#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "cstd.h"
#include "GenVolume.inl"
#include "cube.inl"
#pragma comment(lib,"glew32.lib")
//窗口句柄
static GLint window;
GLuint ProgramObject = 0; //程序对象
GLuint VertexShaderObject = 0; //顶点着色器对象
GLuint FragmentShaderObject = 0; //片段着色器对象
GLfloat xangle, yangle;
GLfloat oldx, oldy;
GLint WinW = 400;
GLint WinH = 400;
GLint g_volTexObj = 0;
//用来检查OpenGL版本，需要GLSL 2.0支持
#include "myglu.inl"
byte* Data = NULL;
int Dim[3] = {256, 256, 256};
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
void getGlVersion(int* major, int* minor)
{
  const char* verstr = (const char*)glGetString(GL_VERSION);
  if ((verstr == NULL) || (sscanf(verstr, "%d.%d", major, minor) != 2)) {
    *major = *minor = 0;
    fprintf(stderr, "Invalid GL_VERSION format!!!\n");
  }
}
static void display(void)
{
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -5.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //查询定点着色器属性变量VertexTemp，并对它进行设置，每个定点需要指定一个属性
  GLint tempLoc = glGetAttribLocationARB(ProgramObject, "VertexTemp");
  // 设置旋转
  glRotatef(xangle, 1.0, 0.0, 0.0);
  glRotatef(yangle, 0.0, 1.0, 0.0);
  // 立方体的8个顶点坐标
  GLfloat vertex_list[][3] = {
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
  };
  GLint index_list[][4] = {
    0, 2, 3, 1,
    0, 4, 6, 2,
    0, 1, 5, 4,
    4, 5, 7, 6,
    1, 3, 7, 5,
    2, 6, 7, 3,
  };
  // 定义不同的颜色
  GLfloat colors[][3] = { { 0.0, 0.0, 1.0 }, { 0.0, 1.0, 0.0 }, { 1.0, 0.0, 0.0 },
    { 1.0, 0.0, 1.0 }, { 1.0, 1.0, 0.0 }, { 0.0, 1.0, 1.0 },
    { 1.0, 0.5, 0.5 }, { 0.0, 0.5, 0.5 }
  };
  int i, j;
  glBegin(GL_QUADS); // 绘制四边形
  for (i = 0; i < 6; ++i) { // 有六个面，循环六次
    glColor3f(colors[i][0], colors[i][1], colors[i][2]);
    for (j = 0; j < 4; ++j) { // 每个面有四个顶点，循环四次
      glVertex3fv(vertex_list[index_list[i][j]]);
    }
  }
  glEnd();
  glFlush();
  glutSwapBuffers();
}
static void reshape(int w, int h)
{
  WinW = w;
  WinH = h;
  float vp = 0.8f;
  float aspect = (float) w / (float) h;
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //glOrtho(-1.0, 1.0, -1.0, 1.0, -10.0, 10.0);
  glFrustum(-vp, vp, -vp / aspect, vp / aspect, 3, 10.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
/*public*/
int installShaders()
{
  GLint vertCompiled, fragCompiled; // status values
  GLint linked;
  /*public*/
  static char* Vertex =
      "//先看顶点着色器temp.vert：\n"
      "uniform float CoolestTemp;\n"
      "uniform float TempRange;\n"
      "attribute float VertexTemp;//每个定点都对应一个温度值\n"
      "varying float Temperature;//传递到片段处理器进行后续处理\n"
      "varying vec4 EntryPoint;\n"
      "varying vec4 ExitPointCoord;\n"
      "void main(void)\n"
      "{\n"
      " //进行插值\n"
      " gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
      " EntryPoint = \n"
      " ExitPointCoord = gl_Position; \n"
      "}\n";
  static char* Fragment =
      "//片段着色器temp.frag：\n"
      "uniform vec3 CoolestColor;\n"
      "uniform vec3 HottestColor;\n"
      "varying float Temperature;\n"
      "varying vec4 EntryPoint;\n"
      "varying vec4 ExitPointCoord;\n"
      "\n"
      "void main(void)\n"
      "{\n"
      " gl_FragColor = vec4(gl_FragCoord.x, 0,0,1.0);\n"
      "}\n";
  // Create a vertex shader object and a fragment shader object
  VertexShaderObject = initShaderObj(Vertex, GL_VERTEX_SHADER);
  FragmentShaderObject = initShaderObj(Fragment, GL_FRAGMENT_SHADER);
  // Create a program object and attach the two compiled shaders
  ProgramObject = glCreateProgram();
  if (0 == linkShader(ProgramObject, VertexShaderObject, FragmentShaderObject)) {
    return 0;
  }
  // Install program object as part of current state
  glUseProgram(ProgramObject);
  // Set up initial uniform values
  glUniform1f(glGetUniformLocation(ProgramObject, "CoolestTemp"), 0.0f);
  glUniform1f(glGetUniformLocation(ProgramObject, "TempRange"), 1.0f);
  glUniform3f(glGetUniformLocation(ProgramObject, "CoolestColor"), 0.0, 0.0, 1.0);
  glUniform3f(glGetUniformLocation(ProgramObject, "HottestColor"), 1.0, 0.0, 0.0);
  return 1;
}
int test_raycast(int argc, char** argv)
{
  int success = 0;
  int gl_major, gl_minor;
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(WinW, WinH);
  window = glutCreateWindow("Temperature Shader");
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glewInit();
  MYREALLOC(Data, 1 << 24);
  GenVolume(Data, Dim);
  g_volTexObj = initVol3DTex((char*)Data, Dim[0], Dim[1], Dim[2], GL_UNSIGNED_BYTE);
  // Initialize the "OpenGL Extension Wrangler" library
  // Make sure that OpenGL 2.0 is supported by the driver
  getGlVersion(&gl_major, &gl_minor);
  printf("GL_VERSION major=%d minor=%d\n", gl_major, gl_minor);
  if (gl_major < 2) {
    printf("GL_VERSION major=%d minor=%d\n", gl_major, gl_minor);
    printf("Support for OpenGL 2.0 is required for this demo...exiting\n");
    exit(1);
  }
  success = installShaders();
  if (success) {
    glutMainLoop();
  }
  return 0;
}

