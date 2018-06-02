#include <GL/glut.h>
#include <math.h>
void myDisplay(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
  glFlush();
}
const GLfloat Pi = 3.1415926536f;
// 画一个圆
/*
正四边形，正五边形，正六边形，……，直到正n边形，当n越大时，这个图形就越接近圆
当n大到一定程度后，人眼将无法把它跟真正的圆相区别
这时我们已经成功的画出了一个“圆”
（注：画圆的方法很多，这里使用的是比较简单，但效率较低的一种）
试修改下面的const int n的值，观察当n=3,4,5,8,10,15,20,30,50等不同数值时输出的变化情况
将GL_POLYGON改为GL_LINE_LOOP、GL_POINTS等其它方式，观察输出的变化情况
*/
void myDisplay1(void)
{
  int i, n = 60;
  double R = 0.5f;;
  glClear(GL_COLOR_BUFFER_BIT);
  //glBegin(GL_POLYGON);
  //glBegin(GL_POINTS);
  glBegin(GL_LINE_LOOP);
  for (i = 0; i < n; ++i) {
    glVertex2f(R * cos(2 * Pi / n * i), R * sin(2 * Pi / n * i));
  }
  glEnd();
  glFlush();
}
// 画一个五角星
/*
设五角星的五个顶点分布位置关系如下：
      A
E        B
    D    C
首先，根据余弦定理列方程，计算五角星的中心到顶点的距离a
（假设五角星对应正五边形的边长为.0）
a = 1 / (2-2*cos(72*Pi/180));
然后，根据正弦和余弦的定义，计算B的x坐标bx和y坐标by，以及C的y坐标
（假设五角星的中心在坐标原点）
bx = a * cos(18 * Pi/180);
by = a * sin(18 * Pi/180);
cy = -a * cos(18 * Pi/180);
五个点的坐标就可以通过以上四个量和一些常数简单的表示出来
*/
void myDisplay2(void)
{
  GLfloat a = 1 / (2 - 2 * cos(72 * Pi / 180));
  GLfloat bx = a * cos(18 * Pi / 180);
  GLfloat by = a * sin(18 * Pi / 180);
  GLfloat cy = -a * cos(18 * Pi / 180);
  GLfloat
  PointA[2] = { 0, a },
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
//画出正弦函数的图形
/*
由于OpenGL默认坐标值只能从-1到1，（可以修改，但方法留到以后讲）
所以我们设置一个因子factor，把所有的坐标值等比例缩小，
这样就可以画出更多个正弦周期
试修改factor的值，观察变化情况
*/
#include <math.h>
const GLfloat factor = 0.1f;
void myDisplay3(void)
{
  GLfloat x;
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_LINES);
  glVertex2f(-1.0f, 0.0f);
  glVertex2f(1.0f, 0.0f);         // 以上两个点可以画x轴
  glVertex2f(0.0f, -1.0f);
  glVertex2f(0.0f, 1.0f);         // 以上两个点可以画y轴
  glEnd();
  glBegin(GL_LINE_STRIP);
  for (x = -1.0f / factor; x < 1.0f / factor; x += 0.01f) {
    glVertex2f(x * factor, sin(x)*factor);
  }
  glEnd();
  glFlush();
}
/*关于点
点的大小默认为1个像素，但也可以改变之。改变的命令为glPointSize，其函数原型如下：
void glPointSize(GLfloat size);
size必须大于0.0f，默认值为1.0f，单位为“像素”。
注意：对于具体的OpenGL实现，点的大小都有个限度的，如果设置的size超过最大值，则设置可能会有问题。
*/
void myDisplay4(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glPointSize(5.999f); //ZB_plot
  glBegin(GL_POINTS);
  glVertex2f(0.0f, 0.0f);
  glVertex2f(0.5f, 0.5f);
  glEnd();
  glFlush();
}
void myDisplay5(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glEnable(GL_LINE_STIPPLE);
  glLineStipple(2, 0x0F0F);
  glLineWidth(10.0f);
  glBegin(GL_LINES);
  glVertex2f(0.0f, 0.0f);
  glVertex2f(0.5f, 0.5f);
  glEnd();
  glFlush();
}
void myDisplay6(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glPolygonMode(GL_FRONT, GL_LINE);  // 设置正面为填充模式
  glPolygonMode(GL_BACK, GL_LINE);   // 设置反面为线形模式
  glFrontFace(GL_CCW);               // 设置逆时针方向为正面
  glBegin(GL_POLYGON);               // 按逆时针绘制一个正方形，在左下方
  glVertex2f(-0.5f, -0.5f);
  glVertex2f(0.0f, -0.5f);
  glVertex2f(0.0f, 0.0f);
  glVertex2f(-0.5f, 0.0f);
  glEnd();
  glBegin(GL_POLYGON);               // 按顺时针绘制一个正方形，在右上方
  glVertex2f(0.0f, 0.0f);
  glVertex2f(0.0f, 0.5f);
  glVertex2f(0.5f, 0.5f);
  glVertex2f(0.5f, 0.0f);
  glEnd();
  glFlush();
}
// 一只苍蝇。
//使用glPolygonStipple来设置镂空的样式。
//其中的参数mask指向一个长度为128字节的空间，
//它表示了一个32*32的矩形应该如何镂 空。
//其中：第一个字节表示了最左下方的从左到右（也可以是从右到左，这个可以修改）
//8个像素是否镂空（1表示不镂空，显示该像素；0表示镂空，显示其后面 的颜色），
//最后一个字节表示了最右上方的8个像素是否镂空。
static GLubyte Mask[128] = {
  0x00, 0x00, 0x00, 0x00,    //   这是最下面的一行
  0x00, 0x00, 0x00, 0x00,
  0x03, 0x80, 0x01, 0xC0,    //   麻
  0x06, 0xC0, 0x03, 0x60,    //   烦
  0x04, 0x60, 0x06, 0x20,    //   的
  0x04, 0x30, 0x0C, 0x20,    //   初
  0x04, 0x18, 0x18, 0x20,    //   始
  0x04, 0x0C, 0x30, 0x20,    //   化
  0x04, 0x06, 0x60, 0x20,    //   ，
  0x44, 0x03, 0xC0, 0x22,    //   不
  0x44, 0x01, 0x80, 0x22,    //   建
  0x44, 0x01, 0x80, 0x22,    //   议
  0x44, 0x01, 0x80, 0x22,    //   使
  0x44, 0x01, 0x80, 0x22,    //   用
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
  0x10, 0x00, 0x00, 0x08    // 这是最上面的一行
};
void myDisplay7(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glEnable(GL_POLYGON_STIPPLE);
  glPolygonStipple(Mask);
  glRectf(-0.5f, -0.5f, 0.0f, 0.0f);   // 在左下方绘制一个有镂空效果的正方形
  glDisable(GL_POLYGON_STIPPLE);
  glRectf(0.0f, 0.0f, 0.5f, 0.5f);     // 在右上方绘制一个无镂空效果的正方形
  glFlush();
}
#if 0
void myDisplay8(void)
{
  int i;
  for (i = 0; i < 8; ++i) {
    auxSetOneColor(i, (float)(i & 0x04), (float)(i & 0x02), (float)(i & 0x01));
  }
  glShadeModel(GL_FLAT);
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(0.0f, 0.0f);
  for (i = 0; i <= 8; ++i) {
    glIndexi(i);
    glVertex2f(cos(i * Pi / 4), sin(i * Pi / 4));
  }
  glEnd();
  glFlush();
}
#endif
void myDisplay9(void)
{
  int i;
  // glShadeModel(GL_FLAT);
  //glShadeModel(GL_SMOOTH);//设置为光滑明暗模式
  glClear(GL_COLOR_BUFFER_BIT);
  if (0) {
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0); //设置第一个顶点为红色
    glVertex2f(-1.0, -1.0); //设置第一个顶点的坐标为（-1.0，-1.0）
    glColor3f(0.0, 1.0, 0.0); //设置第二个顶点为绿色
    glVertex2f(0.0, -1.0); //设置第二个顶点的坐标为（0.0，-1.0）
    glColor3f(0.0, 0.0, 1.0); //设置第三个顶点为蓝色
    glVertex2f(-0.5, 1.0); //设置第三个顶点的坐标为（-0.5，1.0）
    glEnd();
  }
  if (0) {
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(0.0f, 0.0f);
    for (i = 0; i <= 2; ++i) {
      glColor3f(i & 0x04, i & 0x02, i & 0x01);
      glVertex2f(cos(i * Pi / 4), sin(i * Pi / 4));
    }
    glEnd();
  }
  else {
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(0.0f, 0.0f);
    for (i = 0; i <= 8; ++i) {
      glColor3f(i & 0x04, i & 0x02, i & 0x01);
      glVertex2f(cos(i * Pi / 4), sin(i * Pi / 4));
    }
    glEnd();
  }
  glFlush();
}
void myDisplay10(void)
{
  glColor3f(1, 0, 0);
  drawSphere(0, 0, -20, 5, 32, 8);
}
int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(400, 400);
  glFrustum(-10, 10, -10, 10, 5, 60);
  glutCreateWindow("第一个OpenGL程序");
  glutDisplayFunc(myDisplay10);
  glutMainLoop();
  return 0;
}

