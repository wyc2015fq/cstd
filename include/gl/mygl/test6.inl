#include <gl/glut.h>
#define WIDTH 400
#define HEIGHT 400
#include <math.h>
#define ColoredVertex(c, v) do{ glColor3fv(c); glVertex3fv(v); }while(0)   //这段就不用解释了吧……
GLfloat angle = 0.0f; //设定转角
void myDisplay(void)   //绘图函数
{
  static int list = 0;
  if (list == 0) {
    // 如果显示列表不存在，则创建
    GLfloat   //GLfloat为OpenGL用到的数据类型，与C的float基本一致
    PointA[] = { -0.5, -5 * sqrt(5) / 48, sqrt(3) / 6}, //此处为4个顶点的坐标，因为时3D坐标系下的，所以每个坐标有3个分量，分别对应X，Y，Z轴。至于各轴方向定义……默认下屏幕水平为X，竖直为Y，里外为Z。
        PointB[] = { 0.5, -5 * sqrt(5) / 48, sqrt(3) / 6},
            PointC[] = { 0, -5 * sqrt(5) / 48, -sqrt(3) / 3},
                PointD[] = { 0, 11 * sqrt(6) / 48, 0};
    /*GLfloat
    PointA[] = { 0.5f, -sqrt(6.0f)/12, -sqrt(3.0f)/6},
    PointB[] = {-0.5f, -sqrt(6.0f)/12, -sqrt(3.0f)/6},
    PointC[] = { 0.0f, -sqrt(6.0f)/12, sqrt(3.0f)/3},
    PointD[] = { 0.0f, sqrt(6.0f)/4, 0};
    */
    GLfloat
    ColorR[] = {1, 0, 0},   //定义颜色数组，每个数组为一个颜色，也含有3个分量，对应红，绿，蓝，分量范围[0，1]，每种颜色都可看做是这3个颜色混合得到。可一自己改变下其中的数值看看具体效果。
        ColorG[] = {0, 1, 0},
            ColorB[] = {0, 0, 1},
                ColorY[] = {1, 1, 0};
    list = glGenLists(1);
    glNewList(list, GL_COMPILE);  //创建一个顶点表，这个表里包含有绘图的顶点信息
    glBegin(GL_TRIANGLES);   //开始绘图，（GL_TRIANGLES）表示绘制三角形
    // 平面ABC
    ColoredVertex(ColorR, PointA);  //以颜色R绘制点A，以下类推，ColoredVertex（）函数在程序开头定义了。
    ColoredVertex(ColorG, PointB);
    ColoredVertex(ColorB, PointC);
    // 平面ACD
    ColoredVertex(ColorR, PointA);
    ColoredVertex(ColorB, PointC);
    ColoredVertex(ColorY, PointD);
    // 平面CBD
    ColoredVertex(ColorB, PointC);
    ColoredVertex(ColorG, PointB);
    ColoredVertex(ColorY, PointD);
    // 平面BAD
    ColoredVertex(ColorG, PointB);
    ColoredVertex(ColorR, PointA);
    ColoredVertex(ColorY, PointD);
    glEnd();
    glEndList();  //结束绘图 结束绘图顶点表。
    glEnable(GL_DEPTH_TEST);  //打开深度测试。打开深度测试的作用是：如果在场景中有多个物体，而它们相对观察者的距离不同（简单理解为远近），那么这个时候，前面的物体则可以挡住后面的物体（没错吧），使场景具有深度感。如果不打开深度测试，那么绘图会按绘制的顺序，后绘制的物体覆盖住现绘制的物体。这里要注意的是，深度仅影响物体重合时谁显示谁不显示，并不影响其3D效果，远处的物体仍然会变“小”，物体在空间中的位置仍为三维的。
  }
  // 已经创建了显示列表，在每次绘制正四面体时将调用它
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //清除颜色缓存和深度缓存
  glPushMatrix();  //绘图坐标系入栈，可以简单理解为存下了这次画图时画笔的起始位置
  glRotatef(angle, 1, 0.5, 0);  //绘图坐标系绕（1，0.5，0）轴旋转angle角，可理解为模型绕该轴旋转angle角。
  glCallList(list);  //调用顶点表，绘制四面体模型
  glPopMatrix();  //绘图坐标系出栈，则回到了刚才的绘图起始位置
  glutSwapBuffers();  //使用双缓存（第一个在前面显示模型，另一个在后面绘制新的模型，当新模型绘制完毕后送给第一个缓存显示。这样不会出现模型上一些点已经绘制完了，而另一些点还在绘制的情况。）
}
void myIdle(void)
{
  //++angle;
  if (angle >= 360.0f) {
    angle = 0.0f;  //转角超过360度，将其置零。
  }
  myDisplay();  //绘制模型
  Sleep(10);
}
int main(int argc, char* argv[])
{
  glutInit(&argc, argv); //创建绘图窗口
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); //设置绘图模式
  glutInitWindowPosition(200, 200); //设置窗口位置
  glutInitWindowSize(WIDTH, HEIGHT); //设置窗口大小
  glutCreateWindow("OpenGL 窗口"); //设置窗口标题
  glutDisplayFunc(&myDisplay); //重复调用函数
  glutIdleFunc(&myIdle);//重复调用函数
  glutMainLoop(); //该循环用来对上述两个重复调用函数不断调用
  return 0;
}

