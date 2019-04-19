# OpenGL学习笔记：第一个OpenGL程序完全注释 - Koma Hub - CSDN博客
2018年05月05日 11:00:06[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：137
运行环境：
```
CentOS7
g++ (GCC) 4.8.5 20150623 (Red Hat 4.8.5-16)
OpenGL实现厂商的名字：VMware, Inc.
渲染器标识符：Gallium 0.4 on llvmpipe (LLVM 3.9, 256 bits)
OpenGL实现的版本号：2.1 Mesa 17.0.1
OGLU工具库版本：1.3
```
源代码：
```cpp
/**
 * 《OpenGL入门教程》
 *               第一个OpenGL程序
 */
#include <GL/glut.h> 
void myDisplay(void) 
{ 
    /*清楚：颜色*/
    glClear(GL_COLOR_BUFFER_BIT); 
    /*画一个矩形*/
    glRectf(-0.5f, -0.5f, 0.5f, 0.5f); 
    /*保证前面的OpenGL立即执行，不做等待*/
    glFlush(); 
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
    glutCreateWindow("第一个OpenGL程序"); 
    /*设置一个函数*/
    glutDisplayFunc(&myDisplay); 
    /*进行一个消息循环*/
    glutMainLoop(); 
    return 0; 
}
```
编译与运行：
```
$ make
gcc test.c -lGL -lglut -lGLU -lXmu -Bstatic -Bdyanmic
./a.out
```
结果：
![](https://img-blog.csdn.net/20180505105938256)
