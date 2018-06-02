void setLight(void)
{
  static const GLfloat light_position[] = {1.0f, 1.0f, -1.0f, 1.0f};
  static const GLfloat light_ambient[]  = {0.2f, 0.2f, 0.2f, 1.0f};
  static const GLfloat light_diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
  static const GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
}
每一个球体颜色不同。所以它们的材质也都不同。这里用一个函数来设置材质。
void setMatirial(const GLfloat mat_diffuse[4], GLfloat mat_shininess)
{
  static const GLfloat mat_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
  static const GLfloat mat_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
  glMaterialfv(GL_FRONT, GL_EMISSION,  mat_emission);
  glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
}
有了这两个函数，我们就可以根据前面的知识写出整个程序代码了。这里只给出了绘制的部分，其它部分大家可以自行完成。
void myDisplay(void)
{
  // 定义一些材质颜色
  const static GLfloat red_color[] = {1.0f, 0.0f, 0.0f, 1.0f};
  const static GLfloat green_color[] = {0.0f, 1.0f, 0.0f, 0.3333f};
  const static GLfloat blue_color[] = {0.0f, 0.0f, 1.0f, 0.5f};
  // 清除屏幕
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // 启动混合并设置混合因子
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // 设置光源
  setLight();
  // 以(0, 0, 0.5)为中心，绘制一个半径为.3的不透明红色球体（离观察者最远）
  setMatirial(red_color, 30.0);
  glPushMatrix();
  glTranslatef(0.0f, 0.0f, 0.5f);
  glutSolidSphere(0.3, 30, 30);
  glPopMatrix();
  // 下面将绘制半透明物体了，因此将深度缓冲设置为只读
  glDepthMask(GL_FALSE);
  // 以(0.2, 0, -0.5)为中心，绘制一个半径为.2的半透明蓝色球体（离观察者最近）
  setMatirial(blue_color, 30.0);
  glPushMatrix();
  glTranslatef(0.2f, 0.0f, -0.5f);
  glutSolidSphere(0.2, 30, 30);
  glPopMatrix();
  // 以(0.1, 0, 0)为中心，绘制一个半径为.15的半透明绿色球体（在前两个球体之间）
  setMatirial(green_color, 30.0);
  glPushMatrix();
  glTranslatef(0.1, 0, 0);
  glutSolidSphere(0.15, 30, 30);
  glPopMatrix();
  // 完成半透明物体的绘制，将深度缓冲区恢复为可读可写的形式
  glDepthMask(GL_TRUE);
  glutSwapBuffers();
}
