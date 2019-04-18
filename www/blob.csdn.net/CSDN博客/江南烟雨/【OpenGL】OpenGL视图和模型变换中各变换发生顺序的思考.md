# 【OpenGL】OpenGL视图和模型变换中各变换发生顺序的思考 - 江南烟雨 - CSDN博客
2011年12月03日 11:10:11[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：5111
所属专栏：[](https://blog.csdn.net/column/details/.html)
在视图模型变换中，顶点v的变换是按照相反的顺序发生的，而不是按照它的代码中先后顺序出现的。下面举例说明。
考虑下面的代码：
```cpp
glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(N);
	glMultMatrixf(M);
	glMultMatrixf(L);
	glBegin(GL_POINTS);
	glVertex3dv(v);
	glEnd();
```
在这段代码中，模型视图矩阵按顺序分别包含I、N、NM，最后是NML，其中I表示单位矩阵。经过变化的顶点是NMLv。因此顶点变换就是N(M(Lv))。
下面看绘制机器人手臂的display()函数的代码：
```cpp
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glTranslatef(-1.0,0.0,0.0);
	glRotatef((GLfloat)shoulder,0.0,0.0,1.0);
	glTranslatef(1.0,0.0,0.0);
	//绘制第一段手臂
	glPushMatrix(); 
	glScalef(2.0,0.4,1.0);
	glutWireCube(1.0);
	glPopMatrix();
	glTranslatef(1.0,0.0,0.0);
	glRotatef((GLfloat)elbow,0.0,0.0,1.0);
	glTranslatef(1.0,0.0,0.0);
	//绘制第二段手臂
	glPushMatrix(); 
	glScalef(2.0,0.4,1.0);
	glutWireCube(1.0);
	glPopMatrix();
	glPopMatrix();
	glutSwapBuffers();	
}
```
我们通过逐步调试运行来查看效果。
现在注释掉代码：
```cpp
glTranslatef(-1.0,0.0,0.0);
	glRotatef((GLfloat)shoulder,0.0,0.0,1.0);
	glTranslatef(1.0,0.0,0.0);
```
以及绘制第二段手臂的代码：
```cpp
//绘制第二段手臂
	glPushMatrix(); 
	glScalef(2.0,0.4,1.0);
	glutWireCube(1.0);
	glPopMatrix();
```
只保留绘制第一段的代码，便于查看效果。
运行结果：
![](http://hi.csdn.net/attachment/201112/3/0_1322881546N9Vn.gif)
显示的是经过缩放了的立方体。
然后解除下面这行代码的注释：glTranslatef(1.0,0.0,0.0);(位于旋转之下的代码)
运行结果：
![](http://hi.csdn.net/attachment/201112/3/0_1322881579fCBr.gif)
显示的是经过缩放、平移的结果。
在解除这行代码的注释：glRotatef((GLfloat)shoulder,0.0,0.0,1.0);
运行结果：
![](http://hi.csdn.net/attachment/201112/3/0_1322881612zIqt.gif)
显示的是经过缩放、平移、再旋转地结果。
再解除这行代码的注释：glTranslatef(-1.0,0.0,0.0);(位于旋转之上的代码)
运行结果：
![](http://hi.csdn.net/attachment/201112/3/0_1322882139rYlZ.gif)
可以看出是上一步的结果又平移之后的效果。
从上面的单步效果可以看出，乘法的出现顺序和他们的代码中得出现顺序相反。
最后给出绘制两端机器人手臂整个代码：
```cpp
//绘制机器人手臂
#include <GL/glut.h>
static int shoulder = 0,elbow = 0;//两段手臂旋转的角度
void init ()
{
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_FLAT);
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glTranslatef(-1.0,0.0,0.0);
	glRotatef((GLfloat)shoulder,0.0,0.0,1.0);
	glTranslatef(1.0,0.0,0.0);
	//绘制第一段手臂
	glPushMatrix(); 
	glScalef(2.0,0.4,1.0);
	glutWireCube(1.0);
	glPopMatrix();
	glTranslatef(1.0,0.0,0.0);
	glRotatef((GLfloat)elbow,0.0,0.0,1.0);
	glTranslatef(1.0,0.0,0.0);
	//绘制第二段手臂
	glPushMatrix(); 
	glScalef(2.0,0.4,1.0);
	glutWireCube(1.0);
	glPopMatrix();
	glPopMatrix();
	glutSwapBuffers();	
}
void reshape(int w,int h)
{
	glViewport(0,0,(GLsizei)w,(GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0,(GLfloat) w/ (GLfloat)h,1.0,20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0,0.0,-5.0);
}
void keyboard(unsigned char key,int x,int y)
{
	switch(key)//s和e键控制两端手臂的旋转角度
	{
	case 's':
		shoulder = (shoulder + 5)% 360;
		glutPostRedisplay();
		break;
	case 'S':
		shoulder = (shoulder - 5)% 360;
		glutPostRedisplay();
		break;
	case 'e':
		elbow = (elbow + 5) % 360;
		glutPostRedisplay();
		break;
	case 'E':
		elbow = (elbow - 5) % 360;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}
int main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(400,400);
	glutInitWindowPosition(100,100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
```
运行效果(通过s和e键可以控制两端手臂的旋转角度)：
![](http://hi.csdn.net/attachment/201112/3/0_1322881943Io3V.gif)
