# OpenCV学习笔记（六十一）——建立支持OpenGL的OpenCV工程“Master OpenCV”chp.3

 																				2013年01月23日 18:01:05 					[yang_xian521](https://me.csdn.net/yang_xian521) 						阅读数：21811 										

 									

从OpenCV2.4beta版本，OpenGL就可以有接口到highgui的模块中了。结合Master OpenCV第三章的阅读，这里说说如何在OpenCV的显示中嵌入OpenGL的虚拟物体。

要注意的一点：如果想使OpenCV支持OpenGL，不能使用预编译好的library，要用cmake  rebuild工程，注意ENABLE_OPENGL = YES，（在2.4.2版本中，默认ENABLE_OPENGL =  NO），标签的改变在CMake的高级版本都是图形界面的，只需把WITH_OPENGL的对号勾选即可。

这里实战过程中我还遇到了一个问题，用这个CMake得到的vs工程（添加了WITH_OPENGL）无法编译通过，郁闷了好久。因为opengl在vs中是支持的，不需要安装，最后找到了这个bug，需要把\modules\core\src\opengl_interop.cpp文件中使用<gl\gl.h>前面添加#include <windows.h>，（其实#include  <gl.h>前都需要添加#include <windows.h>）这样才能编译通过，这里我只重新编译opencv_core243d.lib 和opencv_highgui243d.lib

已经得到了支持OpenGL的OpenCV lib，接下来就是如何用OpenCV建立OpenGL窗口，基本的调用方式很像OpenCV中鼠标的使用，都是通过回调函数实现，核心代码如下：



```cpp
// callback function
void onDraw(void* param)
{
	// Draw something using OpenGL here
}
 
int main(void)
{
	string openGLWindowName = "OpenGL Test";
	namedWindow(openGLWindowName, WINDOW_OPENGL);
	resizeWindow(openGLWindowName, 640, 480);
	setOpenGlContext(openGLWindowName);
	setOpenGlDrawCallback(openGLWindowName, onDraw, NULL);
 
	updateWindow(openGLWindowName); // when needed
	return 0;
}
```

 以前我们调用

namedWindow

最后一个参数通常会用默认或者使用WINDOW_AUTOSIZE，这回用

WINDOW_OPENGL

，然后调用

setOpenGLContext

建立窗口关联，为了在这个窗口上画虚拟物体，需要使用回调函数，建立方法就是

setOpenGLDrawCallback

，注意这个函数第一个参数是窗口名称，第二个参数是回调函数名，第三个参数是回调函数的参数，因为我这里回调函数onDraw是无参函数，所以这里为NULL。跟MFC重绘需要调用Invalidate或者uadate类似，在需要重绘的时候还要调用

updateWindow

。 



把我做的一个最基础的OpenGL演示上传：<http://download.csdn.net/detail/yang_xian521/5023063>（附上我rebulid的支持OpenGL的lib），效果图如下：

![img](https://img-my.csdn.net/uploads/201301/23/1358934842_4196.png)