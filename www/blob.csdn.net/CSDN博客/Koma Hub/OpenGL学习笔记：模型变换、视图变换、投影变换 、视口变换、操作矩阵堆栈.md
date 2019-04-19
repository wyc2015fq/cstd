# OpenGL学习笔记：模型变换、视图变换、投影变换 、视口变换、操作矩阵堆栈 - Koma Hub - CSDN博客
2018年05月05日 15:44:27[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：172
***1、 模型变换和视图变换 ***
从“相对移动”的观点来看，改变观察点的位置与方向和改变物体本身的位置与方向具有等效性。在OpenGL中，实现这两种功能甚至使用的是同样的函数。 由于模型和视图的变换都通过矩阵运算来实现，在进行变换前，应先设置当前操作的矩阵为“模型视图矩阵”。
设置的方法是以**GL_MODELVIEW**为参数调用**glMatrixMode**函数，像这样： 
```cpp
glMatrixMode(GL_MODELVIEW);
```
通常，我们需要在进行变换前把当前矩阵设置为单位矩阵。
这也只需要一行代码： 
```cpp
glLoadIdentity();
```
然后，就可以进行模型变换和视图变换了。
进行模型和视图变换，主要涉及到三个函数： 
**glTranslate***，把当前矩阵和一个表示移动物体的矩阵相乘。三个参数分别表示了在三个坐标上的位移值。 
**glRotate***，把当前矩阵和一个表示旋转物体的矩阵相乘。物体将绕着(0,0,0)到(x,y,z)的直线以逆时针旋转，参数angle表示旋转的角度。
**glScale***，把当前矩阵和一个表示缩放物体的矩阵相乘。x,y,z分别表示在该方向上的缩放比例。 
注意我都是说“与XX相乘”，而不是直接说“这个函数就是旋转”或者“这个函数就是移动”，这是有原因的，马上就会讲到。 假设当前矩阵为单位矩阵，然后先乘以一个表示旋转的矩阵R，再乘以一个表示移动的矩阵T，最后得到的矩阵再乘上每一个顶点的坐标矩阵v。所以，经过变换得到的顶点坐标就是((RT)v)。由于矩阵乘法的结合率，((RT)v) = (R(Tv))，换句话说，实际上是先进行移动，然后进行旋转。即：实际变换的顺序与代码中写的顺序是相反的。由于“先移动后旋转”和“先旋转后移动”得到的结果很可能不同，初学的时候需要特别注意这一点。 OpenGL之所以这样设计，是为了得到更高的效率。但在绘制复杂的三维图形时，如果每次都去考虑如何把变换倒过来，也是很痛苦的事情。这里介绍另一种思路，可以让代码看起来更自然（写出的代码其实完全一样，只是考虑问题时用的方法不同了）。 让我们想象，坐标并不是固定不变的。旋转的时候，坐标系统随着物体旋转。移动的时候，坐标系统随着物体移动。如此一来，就不需要考虑代码的顺序反转的问题了。
以上都是针对改变物体的位置和方向来介绍的。如果要改变观察点的位置，除了配合使用glRotate*和glTranslate*函数以外，还可以使用这个函数：**gluLookAt**。它的参数比较多，前三个参数表示了观察点的位置，中间三个参数表示了观察目标的位置，最后三个参数代表从(0,0,0)到(x,y,z)的直线，它表示了观察者认为的“上”方向。
***2、投影变换 ***
投影变换就是定义一个可视空间，可视空间以外的物体不会被绘制到屏幕上。（注意，从现在起，坐标可以不再是-1.0到1.0了！） 
OpenGL支持两种类型的投影变换，即透视投影和正投影。投影也是使用矩阵来实现的。如果需要操作投影矩阵，需要以**GL_PROJECTION**为参数调用**glMatrixMode**函数。
```cpp
glMatrixMode(GL_PROJECTION);
```
通常，我们需要在进行变换前把当前矩阵设置为单位矩阵。 
```cpp
glLoadIdentity();
```
透视投影所产生的结果类似于照片，有近大远小的效果，比如在火车头内向前照一个铁轨的照片，两条铁轨似乎在远处相交了。 使用**glFrustum**函数可以将当前的可视空间设置为透视投影空间。其参数的意义如下图：
![](https://img-blog.csdn.net/20180505152743654)
声明：该图片来自www.opengl.org，该图片是《OpenGL编程指南》一书的附图，由于该书的旧版（第一版，1994年）已经流传于网络，我希望没有触及到版权问题。
也可以使用更常用的**gluPerspective**函数。其参数的意义如下图：
![](https://img-blog.csdn.net/20180505152838279)
声明：该图片来自www.opengl.org，该图片是《OpenGL编程指南》一书的附图，由于该书的旧版（第一版，1994年）已经流传于网络，我希望没有触及到版权问题。
正投影相当于在无限远处观察得到的结果，它只是一种理想状态。但对于计算机来说，使用正投影有可能获得更好的运行速度。 
使用glOrtho函数可以将当前的可视空间设置为正投影空间。其参数的意义如下图：
![](https://img-blog.csdn.net/20180505152902490)
声明：该图片来自www.opengl.org，该图片是《OpenGL编程指南》一书的附图，由于该书的旧版（第一版，1994年）已经流传于网络，我希望没有触及到版权问题。 
如果绘制的图形空间本身就是二维的，可以使用gluOrtho2D。他的使用类似于glOrgho。
***3、视口变换 ***
当一切工作已经就绪，只需要把像素绘制到屏幕上了。这时候还剩最后一个问题：应该把像素绘制到窗口的哪个区域呢？通常情况下，默认是完整的填充整个窗口，但我们完全可以只填充一半。（即：把整个图象填充到一半的窗口内）
![](https://img-blog.csdn.net/20180505152932868)
声明：该图片来自www.opengl.org，该图片是《OpenGL编程指南》一书的附图，由于该书的旧版（第一版，1994年）已经流传于网络，我希望没有触及到版权问题。 
使用glViewport来定义视口。其中前两个参数定义了视口的左下脚（0,0表示最左下方），后两个参数分别是宽度和高度。
***4、操作矩阵堆栈 ***
介于是入门教程，先简单介绍一下堆栈。你可以把堆栈想象成一叠盘子。开始的时候一个盘子也没有，你可以一个一个往上放，也可以一个一个取下来。每次取下的，都是最后一次被放上去的盘子。通常，在计算机实现堆栈时，堆栈的容量是有限的，如果盘子过多，就会出错。当然，如果没有盘子了，再要求取一个盘子，也会出错。 
我们在进行矩阵操作时，有可能需要先保存某个矩阵，过一段时间再恢复它。当我们需要保存时，调用glPushMatrix函数，它相当于把矩阵（相当于盘子）放到堆栈上。当需要恢复最近一次的保存时，调用glPopMatrix函数，它相当于把矩阵从堆栈上取下。OpenGL规定堆栈的容量至少可以容纳32个矩阵，某些OpenGL实现中，堆栈的容量实际上超过了32个。因此不必过于担心矩阵的容量问题。 
通常，用这种先保存后恢复的措施，比先变换再逆变换要更方便，更快速。 
注意：模型视图矩阵和投影矩阵都有相应的堆栈。使用glMatrixMode来指定当前操作的究竟是模型视图矩阵还是投影矩阵。
***5.举例***
**以太阳、地球和月球三者的旋转关系，我个人有扩展了书《OpenGL入门教程(精)》中进行了扩展，添加了动画功能，在键盘上按“f”则时间day++，按“b”则时间day--。**
**源代码：**
```cpp
/**
 * 《OpenGL入门教程》
 *               太阳、地球、月亮转啊转
 */
#include <GL/glut.h> 
/* 初始化材料属性、光源属性、光照模型，打开深度缓冲区 */
void init ( void )
{
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
// 一年12个月，共是360天 
static int day = 100; 
// day的变化：从0到359 
void myDisplay(void) 
{ 
    glEnable(GL_DEPTH_TEST); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 
    gluPerspective(75, 1, 1, 400000000); 
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity(); 
    gluLookAt(0, -200000000, 200000000, 0, 0, 0, 0, 0, 1);
    // 绘制红色的“太阳” 
    glColor3f(1.0f, 0.0f, 0.0f); 
    glutSolidSphere(69600000, 20, 20); 
    // 绘制蓝色的“地球” 
    glColor3f(0.0f, 0.0f, 1.0f); 
    glRotatef(day/360.0*360.0, 0.0f, 0.0f, -1.0f); 
    glTranslatef(150000000, 0.0f, 0.0f); 
    glutSolidSphere(15945000, 20, 20); 
    // 绘制黄色的“月亮” 
    glColor3f(1.0f, 1.0f, 0.0f); 
    glRotatef(day/30.0*360.0 - day/360.0*360.0, 0.0f, 0.0f, -1.0f); 
    glTranslatef(38000000, 0.0f, 0.0f); 
    glutSolidSphere(4345000, 20, 20);
    glFlush(); 
}
/* ARGSUSED1 */
void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
      case 'f':
         day+=1;
            /*设置一个函数*/
         glutPostRedisplay(); 
         break;
      case 'b':
         day-=1;
            /*设置一个函数*/
         glutPostRedisplay(); 
         break;
      case 27:
         exit(0);  /*  Escape key  */
         break;
      default:
         break;
   }
}
int main(int argc, char *argv[]) 
{ 
    /*初始化*/
    glutInit(&argc, argv); 
    /*设置显示方式：RGB颜色，单缓冲*/
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE); 
    /*设置在窗口的中心位置*/
    glutInitWindowPosition(100, 100); 
    /*设置窗口大小*/
    glutInitWindowSize(400, 400); 
    /*创建窗口*/
    glutCreateWindow("Sun Earth Moon");
    /* 调用OpenGL初始化函数 */
    init ( ); 
    glutKeyboardFunc (&keyboard);
    glutDisplayFunc(&myDisplay);  
    /*进行一个消息循环*/
    glutMainLoop(); 
    return 0; 
}
```
**我的运行环境：**
```
CentOS7
g++ (GCC) 4.8.5 20150623 (Red Hat 4.8.5-16)
OpenGL实现厂商的名字：VMware, Inc.
渲染器标识符：Gallium 0.4 on llvmpipe (LLVM 3.9, 256 bits)
OpenGL实现的版本号：2.1 Mesa 17.0.1
OGLU工具库版本：1.3
```
**编译与运行：**
```
$ make
gcc test.c -lGL -lglut -lGLU -lXmu -Bstatic -Bdyanmic -lm
./a.out
```
**效果图：**
![](https://img-blog.csdn.net/2018050515403676)
部分代码以及内容来自《OpenGL入门教程(精)》，如有侵权，请联系删除。
