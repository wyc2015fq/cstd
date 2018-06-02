//************************************
//才才
//球的光影效果
//2010.5.10
//************************************
void init(void)
{
  GLfloat mat_sp[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat mat_sh[] = {50.0};
  GLfloat light_p[] = {1, 1, 1, 0};
  GLfloat yellow_l[] = {1, 1, 0, 1};
  GLfloat lmodel_a[] = {0.1, 0.1, 0.1, 1.0};
  glClearColor(0, 0, 0, 0);
  glShadeModel(GL_SMOOTH);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_sp);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_sh);
  glLightfv(GL_LIGHT0, GL_POSITION, light_p); //指定光源的位置
  glLightfv(GL_LIGHT0, GL_DIFFUSE, yellow_l); //设定漫反射效果
  glLightfv(GL_LIGHT0, GL_SPECULAR, yellow_l); //设定高光反射效果
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_a); //设定全局环境光
  glEnable(GL_LIGHTING); //启用光源
  glEnable(GL_LIGHT0);   //使用指定灯光
  glEnable(GL_DEPTH_TEST);
}
void myDisplay(void)
{
  glClearColor(1, 1, 1, 0); //设置背景色
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //以上面颜色清屏并清除深度缓存
  glutSolidSphere(1.0, 4, 4); //画一个球体
  glFlush();
}
void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (w <= h) {
    glOrtho(-1.5, 1.5, -1.5 * (GLfloat) h / (GLfloat) w, 1.5 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
  }
  else {
    glOrtho(-1.5 * (GLfloat)w / (GLfloat)h, 1.5 * (GLfloat) w / (GLfloat) h, -1.5, 1.5, -10.0, 10.0);
  }
}
int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
  glutInitWindowSize(500, 500);    //显示框大小
  glutInitWindowPosition(200, 400); //确定显示框左上角的位置
  glutCreateWindow("球的光影效果——才才");
  init();
  glutDisplayFunc(myDisplay);
  glutReshapeFunc(reshape);
  glutMainLoop();
  return 0;
}

