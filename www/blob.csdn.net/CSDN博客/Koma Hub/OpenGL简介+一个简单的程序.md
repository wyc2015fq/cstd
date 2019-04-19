# OpenGL简介+一个简单的程序 - Koma Hub - CSDN博客
2018年05月03日 23:04:38[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：139
OpenGL(全写Open Graphics Library)
![](https://img-blog.csdn.net/20180503225607990)
OpenGL是指定义了一个跨编程语言、跨平台的编程接口规格的专业的图形程序接口。它用于三维图像(二维的亦可)，是一个功能强大，调用方便的底层图形库。
OpenGL是行业领域中最为广泛接纳的 2D/3D 图形 API，其自诞生至今已催生了各种计算机平台及设备上的数千优秀应用程序。OpenGL是独立于视窗操作系统或其它操作系统的，亦是网络透明的。在包含CAD、内容创作、能源、娱乐、游戏开发、制造业、制药业及虚拟现实等行业领域中，OpenGL帮助程序员实现在 PC、工作站、超级计算机等硬件设备上的高性能、极具冲击力的高视觉表现力图形处理软件的开发。
一个简单的OpenGL程序。
环境：CentOS7
g++ (GCC) 4.8.5 20150623 (Red Hat 4.8.5-16)
OpenGL实现厂商的名字：VMware, Inc.
渲染器标识符：Gallium 0.4 on llvmpipe (LLVM 3.9, 256 bits)
OpenGL实现的版本号：2.1 Mesa 17.0.1
OGLU工具库版本：1.3
```cpp
/** 
 *      绘制一个光照的球。
 *
 *           test.c 2018.05
 *由于头文件glut.h中已经包含了头文件gl.h和glu.h，所以只需要include 此文件
 */
# include <GL/glut.h>
# include <stdlib.h>
/* 初始化材料属性、光源属性、光照模型，打开深度缓冲区 */
void init ( void )
{
    const GLubyte* name = glGetString(GL_VENDOR); 
    const GLubyte* biaoshifu = glGetString(GL_RENDERER);
    const GLubyte* OpenGLVersion =glGetString(GL_VERSION);
    const GLubyte* gluVersion= gluGetString(GLU_VERSION);
    printf("OpenGL实现厂商的名字：%s\n", name);
    printf("渲染器标识符：%s\n", biaoshifu);
    printf("OpenGL实现的版本号：%s\n",OpenGLVersion );
    printf("OGLU工具库版本：%s\n", gluVersion);
    GLfloat mat_specular [ ] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess [ ] = { 50.0 };
    GLfloat light_position [ ] = { 1.0, 1.0, 1.0, 0.0 };
    glClearColor ( 0.0, 0.0, 0.0, 0.0 );
    glShadeModel ( GL_SMOOTH );
    glMaterialfv ( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv ( GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv ( GL_LIGHT0, GL_POSITION, light_position);
    
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
    glEnable (GL_DEPTH_TEST);
}
/*调用GLUT函数，绘制一个球*/
void display ( void )
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutSolidSphere (1.0, 40, 50);
    glFlush ();
}
/* 定义GLUT的reshape函数，w、h分别是当前窗口的宽和高*/
void reshape (int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ( );
    if (w <= h)
        glOrtho (-1.5, 1.5, -1.5 * ( GLfloat ) h / ( GLfloat ) w, 
                    1.5 * ( GLfloat ) h / ( GLfloat ) w, -10.0, 10.0 );
    else
        glOrtho (-1.5 * ( GLfloat ) w / ( GLfloat ) h, 
                    1.5 * ( GLfloat ) w / ( GLfloat ) h, 
                    -1.5, 1.5, -10.0, 10.0);
    glMatrixMode ( GL_MODELVIEW );
    glLoadIdentity ( ) ;
}
/* 定义对键盘的响应函数 */
void keyboard ( unsigned char key, int x, int y)
{
    /*按Esc键退出*/
    switch (key) 
    {
    case 27:
        exit ( 0 );
        break;
    }
}
int main(int argc, char** argv)
{
    /* GLUT环境初始化*/
    glutInit (&argc, argv);
    /* 显示模式初始化 */
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    /* 定义窗口大小 */
    glutInitWindowSize (300, 300);
    /* 定义窗口位置 */
    glutInitWindowPosition (100, 100);
    /* 显示窗口，窗口标题为执行函数名 */
    glutCreateWindow ( argv [ 0 ] );
    /* 调用OpenGL初始化函数 */
    init ( );
    /* 注册OpenGL绘图函数 */
    glutDisplayFunc ( display );
    /* 注册窗口大小改变时的响应函数 */
    glutReshapeFunc ( reshape );
    /* 注册键盘响应函数 */
    glutKeyboardFunc ( keyboard );
    /* 进入GLUT消息循环，开始执行程序 */
    glutMainLoop( );
    return 0;
}
```
Makefile文件：
```
ALL:
	gcc test.c -lGL -lglut -lGLU -lXmu -Bstatic -Bdyanmic
	./a.out
clean:
	rm *~
```
效果图：
![](https://img-blog.csdn.net/20180503230350148)
