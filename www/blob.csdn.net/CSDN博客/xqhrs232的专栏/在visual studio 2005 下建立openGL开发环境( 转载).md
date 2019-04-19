# 在visual studio 2005 下建立openGL开发环境( 转载) - xqhrs232的专栏 - CSDN博客
2009年11月05日 13:46:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1176标签：[windows																[application																[direct3d																[图形																[buffer																[语言](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=图形&t=blog)](https://so.csdn.net/so/search/s.do?q=direct3d&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[3D/OpenGL](https://blog.csdn.net/xqhrs232/article/category/906930)
 OpenGL入门学习-----第一课----编写第一个OpenGL程序
OpenGL作为当前主流的图形API之一，它在一些场合具有比DirectX更优越的特性。
1、与C语言紧密结合。
OpenGL命令最初就是用C语言函数来进行描述的，对于学习过C语言的人来讲，OpenGL是容易理解和学习的。如果你曾经接触过TC的graphics.h，你会发现，使用OpenGL作图甚至比TC更加简单。
2、强大的可移植性。
微软的Direct3D虽然也是十分优秀的图形API，但它只用于Windows系统（现在还要加上一个XBOX游戏机）。而OpenGL不仅用于Windows，还可以用于Unix/Linux等其它系统，它甚至在大型计算机、各种专业计算机（如：医疗用显示设备）上都有应用。并且，OpenGL的基本命令都做到了硬件无关，甚至是平台无关。
3、高性能的图形渲染。
OpenGL是一个工业标准，它的技术紧跟时代，现今各个显卡厂家无一不对OpenGL提供强力支持，激烈的竞争中使得OpenGL性能一直领先。
总之，OpenGL是一个很NB的图形软件接口。至于究竟有多NB，去看看DOOM3和QUAKE4等专业游戏就知道了。
OpenGL官方网站（英文）
[http://www.opengl.org](http://www.opengl.org)
下面我将对Windows下的OpenGL编程进行简单介绍。
学习OpenGL前的准备工作
第一步，选择一个编译环境
现在Windows系统的主流编译环境有Visual Studio，Broland C++ Builder，Dev-C++等，它们都是支持OpenGL的。但这里我们选择Visual Studio 2005作为学习OpenGL的环境。
第二步，安装GLUT工具包
GLUT不是OpenGL所必须的，但它会给我们的学习带来一定的方便，推荐安装。
Windows环境下的GLUT下载地址：（大小约为150k）
[http://www.opengl.org/resources/libraries/glut/glutdlls37beta.zip](http://www.opengl.org/resources/libraries/glut/glutdlls37beta.zip)
无法从以上地址下载的话请使用下面的连接:
[http://upload.programfan.com/upfile/200607311626279.zip](http://upload.programfan.com/upfile/200607311626279.zip)
Windows环境下安装GLUT的步骤：
1、将下载的压缩包解开，将得到5个文件
2、在“我的电脑”中搜索“gl.h”，并找到其所在文件夹（如果是VisualStudio2005，则应该是其安装目录下面的“VC/PlatformSDK/include/gl文件夹”）。把解压得到的glut.h放到这个文件夹。
3、把解压得到的glut.lib和glut32.lib放到静态函数库所在文件夹（如果是VisualStudio2005，则应该是其安装目录下面的“VC/lib”文件夹）。
4、把解压得到的glut.dll和glut32.dll放到操作系统目录下面的system32文件夹内。（典型的位置为：C:/Windows/System32）
第三步，建立一个OpenGL工程
这里以VisualStudio2005为例。
选择File->New->Project，然后选择Win32 Console Application，选择一个名字，然后按OK。
在谈出的对话框左边点Application Settings，找到Empty project并勾上，选择Finish。
然后向该工程添加一个代码文件，取名为“OpenGL.c”，注意用.c来作为文件结尾。
搞定了，就跟平时的工程没什么两样的。
第一个OpenGL程序
一个简单的OpenGL程序如下：（注意，如果需要编译并运行，需要正确安装GLUT，安装方法如上所述）
#include <GL/glut.h>
void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
    glFlush();
}
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("第一个OpenGL程序");
    glutDisplayFunc(&myDisplay);
    glutMainLoop();
    return 0;
}
该程序的作用是在一个黑色的窗口中央画一个白色的矩形。下面对各行语句进行说明。
首先，需要包含头文件#include <GL/glut.h>，这是GLUT的头文件。
本来OpenGL程序一般还要包含<GL/gl.h>和<GL/glu.h>，但GLUT的头文件中已经自动将这两个文件包含了，不必再次包含。
然后看main函数。
int main(int argc, char *argv[])，这个是带命令行参数的main函数，各位应该见过吧？没见过的同志们请多翻翻书，等弄明白了再往下看。
注意main函数中的各语句，除了最后的return之外，其余全部以glut开头。这种以glut开头的函数都是GLUT工具包所提供的函数，下面对用到的几个函数进行介绍。
1、glutInit，对GLUT进行初始化，这个函数必须在其它的GLUT使用之前调用一次。其格式比较死板，一般照抄这句glutInit(&argc, argv)就可以了。
2、glutInitDisplayMode，设置显示方式，其中GLUT_RGB表示使用RGB颜色，与之对应的还有GLUT_INDEX（表示使用索引颜色）。GLUT_SINGLE表示使用单缓冲，与之对应的还有GLUT_DOUBLE（使用双缓冲）。更多信息，请自己Google。当然以后的教程也会有一些讲解。
3、glutInitWindowPosition，这个简单，设置窗口在屏幕中的位置。
4、glutInitWindowSize，这个也简单，设置窗口的大小。
5、glutCreateWindow，根据前面设置的信息创建窗口。参数将被作为窗口的标题。注意：窗口被创建后，并不立即显示到屏幕上。需要调用glutMainLoop才能看到窗口。
6、glutDisplayFunc，设置一个函数，当需要进行画图时，这个函数就会被调用。（这个说法不够准确，但准确的说法可能初学者不太好理解，暂时这样说吧）。
7、glutMainLoop，进行一个消息循环。（这个可能初学者也不太明白，现在只需要知道这个函数可以显示窗口，并且等待窗口关闭后才会返回，这就足够了。）
在glutDisplayFunc函数中，我们设置了“当需要画图时，请调用myDisplay函数”。于是myDisplay函数就用来画图。观察myDisplay中的三个函数调用，发现它们都以gl开头。这种以gl开头的函数都是OpenGL的标准函数，下面对用到的函数进行介绍。
1、glClear，清除。GL_COLOR_BUFFER_BIT表示清除颜色，glClear函数还可以清除其它的东西，但这里不作介绍。
2、glRectf，画一个矩形。四个参数分别表示了位于对角线上的两个点的横、纵坐标。
3、glFlush，保证前面的OpenGL命令立即执行（而不是让它们在缓冲区中等待）。其作用跟fflush(stdout)类似。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/java641/archive/2009/07/17/4356897.aspx](http://blog.csdn.net/java641/archive/2009/07/17/4356897.aspx)
