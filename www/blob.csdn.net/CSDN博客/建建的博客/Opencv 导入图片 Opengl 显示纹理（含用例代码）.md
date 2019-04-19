# Opencv 导入图片 Opengl 显示纹理（含用例代码） - 建建的博客 - CSDN博客
2017年08月14日 18:08:34[纪建](https://me.csdn.net/u013898698)阅读数：940
个人分类：[openGL](https://blog.csdn.net/u013898698/article/category/7028208)

本篇主要是讲使用Opencv导入一副图片，然后讲该副图片当成纹理显示在opengl当中。
- 
## 首先说如何配置Opencv
我的硬件环境是： Windows10+Visual Studio 2015+opencv3.0
先在[此网站](https://sourceforge.net/projects/opencvlibrary/files/opencv-win/3.0.0/opencv-3.0.0.exe/download?use_mirror=nchc)下载opencv,下载之后，它是长这样的![](http://images2015.cnblogs.com/blog/1010311/201704/1010311-20170427140635631-838292639.png),点击它，会自动解压缩成名字为“opencv”的文件夹
接下来进行环境配置：从控制面板开始，按着这个路径  Control Panel\System and Security\System 来到高级系统设置（advanced system settings）就进入了如下界面：
![](http://images2015.cnblogs.com/blog/1010311/201704/1010311-20170427141014178-1558404158.png)
在系统变量里面找到Path,然后在Value中添加OpenCV的目录：我的是：C:\Users\Dell\Downloads\opencv\build\x86\vc12\bin。（我的opencv直接保存在了downloads里面）
然后你新建一个VS2015的项目（新建项目方法我就不说了），然后点击：项目--->属性--->配置属性--->VC++目录----->包含目录
把下面这些目录添加进去：
C:\Users\Dell\Downloads\opencv\build\include
C:\Users\Dell\Downloads\opencv\build\include\opencv
C:\Users\Dell\Downloads\opencv\build\include\opencv2
最后大概长这样：
![](http://images2015.cnblogs.com/blog/1010311/201704/1010311-20170427141712490-1792981264.png)
再在库目录中添加这些目录：
C:\Users\Dell\Downloads\opencv\build\x86\vc12\lib
C:\Users\Dell\Downloads\opencv\build\x86\vc12\staticlib
 注意：你可以发现我这里面用的是x86架构，其实opencv里面还给了x64架构。这主要取决于你的项目用的是什么架构，可以看这里来判断
![](http://images2015.cnblogs.com/blog/1010311/201704/1010311-20170427142047944-1021178048.png)
最后一步：
点击链接器---> 输入---->附加依赖项中加入：
opencv_ts300.lib; opencv_world300.lib
至此 opencv就配置好了。你当然也要配置opengl，但是这里我就不说了。
## 2. 图片作为纹理在opengl中显示
先把要用的资源图片放在你VS项目中的目录中，与cpp文件一个目录下面。
请复制一下代码：
```
#include<gl/glut.h>
#include <opencv.hpp>
using namespace cv;
//OpenCV读取图像
Mat I = imread("ntulogo.bmp");
//设置长宽
int width = I.cols;
int height = I.rows;
//设置图像指针
GLubyte* pixels;
GLuint load_texture()
{
    //OpenGL纹理用整型数表示
    GLuint texture_ID;
    //获取图像指针
    int pixellength = width*height * 3;
    pixels = new GLubyte[pixellength];
    memcpy(pixels, I.data, pixellength * sizeof(char));
    imshow("OpenCV", I);
    //将texture_ID设置为2D纹理信息
    glGenTextures(1, &texture_ID);
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    //纹理放大缩小使用线性插值
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //纹理水平竖直方向外扩使用重复贴图
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //纹理水平竖直方向外扩使用边缘像素贴图(与重复贴图二选一)
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    //将图像内存用作纹理信息
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
    free(pixels);
    return texture_ID;
}
void display()
{
    // 清除屏幕
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //获取纹理对象
    GLuint image = load_texture();
    //重新设置OpenGL窗口：原点位置为左上角，x轴从左到右，y轴从上到下，坐标值与像素坐标值相同
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);
    //显示纹理
    glEnable(GL_TEXTURE_2D);    //允许使用纹理
    glBindTexture(GL_TEXTURE_2D, image);    //选择纹理对象
                                            //原始完全填充四边形
    glBegin(GL_POLYGON);    //设置为多边形纹理贴图方式并开始贴图
    glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);    //纹理左上角对应窗口左上角
    glTexCoord2f(0.0f, 1.0f); glVertex2f(0, height);    //纹理左下角对应窗口左下角
    glTexCoord2f(1.0f, 1.0f); glVertex2f(width, height);    //纹理右下角对应窗口右下角
    glTexCoord2f(1.0f, 0.0f); glVertex2f(width, 0);    //纹理右上角对应窗口右上角
    glEnd();    //结束贴图*/
                /*//三角形
                glBegin(GL_POLYGON);
                glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
                glTexCoord2f(0.0f, 1.0f); glVertex2f(0, height);
                glTexCoord2f(1.0f, 1.0f); glVertex2f(width, height);
                glEnd();*/
            /*    //多边形
                glBegin(GL_POLYGON);
                glTexCoord2f(0, 0); glVertex2f(0, 0);
                glTexCoord2f(0, 1); glVertex2f(0, height/3);
                glTexCoord2f(1, 1); glVertex2f(width/4, height/3);
                glTexCoord2f(1, 0); glVertex2f(width/4, 0);
                glTexCoord2f(0.0f, 0.5f); glVertex2f(0, height/2);
                glEnd();*/
                /*//任意变换
                glBegin(GL_POLYGON);
                glTexCoord2f(0.0f, 0.0f); glVertex2f(width/4, height/4);
                glTexCoord2f(0.0f, 1.0f); glVertex2f(0, height);
                glTexCoord2f(1.0f, 1.0f); glVertex2f(width, height*2/3);
                glTexCoord2f(1.0f, 0.0f); glVertex2f(width*4/5, 50);
                glEnd();*/
                //边缘贴图效果
                /*glBegin(GL_POLYGON);
                glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
                glTexCoord2f(0.0f, 2.0f); glVertex2f(0, height);
                glTexCoord2f(2.0f, 2.0f); glVertex2f(width, height);
                glTexCoord2f(2.0f, 0.0f); glVertex2f(width, 0);
                glEnd();*/
    glDisable(GL_TEXTURE_2D);    //禁止使用纹理
                                //双缓存交换缓存以显示图像
    glutSwapBuffers();
}
void main(int argc, char** argv)
{
    //初始化GL
    glutInit(&argc, argv);
    //设置显示参数(双缓存，RGB格式)
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    //设置窗口尺寸：width*height
    glutInitWindowSize(width, height);
    //设置窗口位置：在屏幕左上角像素值(100,100)处
    glutInitWindowPosition(100, 100);
    //设置窗口名称
    glutCreateWindow("OpenGL");
    //显示函数，display事件需要自行编写
    glutDisplayFunc(display);
    //重复循环GLUT事件
    glutMainLoop();
}
```
最后你会看到这个结果：
![](http://images2015.cnblogs.com/blog/1010311/201704/1010311-20170427142959412-64660395.png)
## 3. 再说几个小问题
如果在opengl中，你在使用贴纹理之前，有使用过glColor函数，就会影响到接下来的贴图，例如：我前面使用过了glColor3f(1,0,0).那么我的纹理图就会变成这样：
![](http://images2015.cnblogs.com/blog/1010311/201704/1010311-20170427143717287-235842859.png)
所以，需要在纹理贴图之前，写一句glColor3f(1,1,1)
其次如果你的项目中用到了光照，那这个光照可能会影响到纹理的显示，就会变暗。
解决方法是在画纹理的前后加上，关闭光照和打开光照这两句话，即：
glDisable(GL_LIGHTING);
glEnable(GL_LIGHTING);
 还有不懂之处，请参考一下参考网站
reference：
1. https://zhuanlan.zhihu.com/p/24714986?refer=ry000
2. http://blog.csdn.net/lanergaming/article/details/48689841
3. https://learnopengl-cn.readthedocs.io/zh/latest/01%20Getting%20started/06%20Textures/
