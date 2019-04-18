# openGL常用的函数 - nosmatch的专栏 - CSDN博客
2010年03月29日 18:52:00[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：6805
**glViewport()**函数****
在OpenGL初始化完成之后，我们应该进行一些视图设置。首先是设定视见区域，即告诉OpenGL应把渲染之后的图形绘制在窗体的哪个部位。当视见区域是整个窗体时，OpenGL将把渲染结果绘制到整个窗口。我们调用glViewPort函数来决定视见区域: 
procedure glViewPort(x:GLInt;y:GLInt;Width:GLSizei;Height:GLSizei);
其中，参数X，Y指定了视见区域的左下角在窗口中的位置，一般情况下为（0，0），Width和Height指定了视见区域的宽度和高度。注意OpenGL使用的窗口坐标和WindowsGDI使用的窗口坐标是不一样的。图3.1-1表示了在WindowsGDI中的窗口坐标，而图3.1-2则是OpenGL所定义的窗口坐标。
![](http://hiphotos.baidu.com/xun1573/pic/item/80ccba33336a055fac4b5fec.jpg)![](http://hiphotos.baidu.com/xun1573/pic/item/10706df4fed07872ddc474ea.jpg)
例如，要设置如图3.1-3中的视见区域，我们应该调用函数:
      glViewPort(100,100,Width,Height);
![](http://hiphotos.baidu.com/xun1573/pic/item/62876bfa1035f38b59ee90f5.jpg)
**glMatrixMode()**
指定哪一个矩阵是当前矩阵
C语言描述void glMatrixMode(GLenum mode)
参数mode指定哪一个矩阵堆栈是下一个矩阵操作的目标，可选值:GL_MODELVIEW、GL_PROJECTION、GL_TEXTURE.
说明
glMatrixMode设置当前矩阵模式：
GL_MODEVIEW，对模型视景矩阵堆栈应用随后的矩阵操作。
GL_PROJECTION，对投影矩阵应用随后的矩阵操作。
GLTEXTURE，对纹理矩阵堆栈应用随后的矩阵操作。
**glLoadIdentity()**
该函数的功能是重置当前指定的矩阵为单位矩阵。在glLoadIdentity()之后我们为场景设置了透视图。glMatrixMode(GL_MODELVIEW)设置当前矩阵为模型视图矩阵，模弄视图矩阵储存了有关物体的信。
**gluOrtho2D()**
gluOrtho2D(-5.0,5.0,-5.0,5.0); 
参数分别代表（左下角x坐标，右上角x坐标，左下角y坐标，右上角y坐标）——坐标全相对于窗口左下角－－原点），near和far默认为-1和1，此函 数决定一个平行六面体，即View Volume！     View Volume越大，里面的物体显得越小！如，一个点的坐标是(0,0,0)就是在平行六面体的中间，也就是在viewport的中间;又如一个点的坐标是(-5.0,-5.0,0),是在平行六面体的左下角，也就是在vieport的左下角。
注：viewport是指视窗的大小，就好像我们的眼睛！viweport可以用下面函数指定：glViewport(左下角x坐标, 左下角y坐标, wigth, height); ，默认是（0，0，窗口的宽度，窗口的高度）可以用gluOrtho函数来指定near和far，gluOrtho（Xmin,Xmax,Ymin,Ymax,near,far);
**glTranslatef()**
glTranslatef的作用就是移动坐标原点。对应的3个参数对应着3个坐标轴。如果你调用一次glTranslatef(1.0f,0.0f,0.0f)然后画一个小球，接着再调用次glTranslatef(0.0f,1.0f,0.0f)再画一个小球。此时，两个小球中，一个在另外一个正右方。所以，如果要使两个小球分别处于x、y轴，则需要在第二次画之前调用glLoadIdentity()函数，使坐标原点归位。另外，此处的坐标系为右手坐标系。
**glPushMatrix() glPopMatrix()**
相当于栈里的入栈和出栈。
许多人不明白的可能是入的是什么，出的又是什么。我也是自己反复做了下测试才懂的.例如你当前的坐标系原点在你电脑屏幕的左上方。现在你调用glPushMatrix，然后再调用一堆平移、旋转代码等等，然后再画图。那些平移和旋转都是基于坐上角为原点进行变化的。而且都会改变坐标的位置，经过了这些变化后，你的坐标肯定不再左上角了。那如果想恢复怎么办呢？这时就调用glPopMatrix从栈里取出一个“状态”了，这个状态就是你调用glPushMatrix之前的那个状态。就如很多opengl的书上所讲：调用glPushMatrix其实就是把当前状态做一个副本放入堆栈之中。
**glRasterPos2i()**
glRasterPos2i(200, 200); 改变光栅位置
光栅（Raster）：由像素构成的一个矩形网格。要在光栅上显示的数据保存于帧缓存内。
**glBitmap()**当设置了光栅位置后，就可以调用glBitmap()函数来显示位图数据了。这个函数形式为：
void glBitmap( GLsizei width,GLsizei height,GLfloat xbo,GLfloat ybo,GLfloat xbi,GLfloat ybi,const GLubyte *bitmap);
显示由bitmap指定的位图，bitmap是一个指向位图的指针。位图的原点放在 最近定义的当前光栅位置上。若当前光栅位置是无效的，则不显示此位图或其一部分，而且当前光栅位置仍然无效。参数width和height一象素为单位说 明位图的宽行高。宽度不一定是8的倍数。参数xbo和ybo定义位图的原点（正值时，原点向上移动；负值时，原点向下移动）。参数xbi和ybi之处在位 图光栅化后光栅位置的增量。
```cpp
**glReadPixels()**读取象素数据,void glReadPixels(GLint x,GLint y,GLsizesi width,GLsizei height,
GLenum format,GLenum type,GLvoid *pixel);
函数参数(x,y)定义图像区域左下角点的坐标，width和height分别是图像的高度和宽度，*pixel是一个指针，
指向存储图像数据的数组。参数format指出所读象素数据元素的格式（索引值或R、G、B、A值，如
表11-1所示），而参数type指出每个元素的数据类型（见表11-2）。
名称                 象素数据类型
GL_INDEX          单个颜色索引
GL_RGB                  先是红色分量，再是绿色分量，然后是蓝色分量
GL_RED                  单个红色分量
GL_GREEN          单个绿色分量
GL_BLUE          单个蓝色分量
GL_ALPHA          单个Alpha值
GL_LUMINANCE_ALPHA  先是亮度分量，然后是Alpha值
GL_STENCIL_INDEX  单个的模板索引
GL_DEPTH_COMPONENT  单个深度分量
表11-1 函数glReadPixels()及glDrawPixels()的象素格式
名称             数据类型
GL_UNSIGNED_BYTE    无符号的8位整数
GL_BYTE      8位整数
GL_BITMAP      无符号的8位整数数组中的单个数位
GL_UNSIGNED_SHORT   无符号的16位整数
GL_SHORT      16位整数
GL_UNSIGNED_INT     无符号的32位整数
GL_INT              32位整数
GL_FLOAT      单精度浮点数
表11-2 函数glReadPixels()及glDrawPixels()的象素数据类型
```
```cpp
**glDrawPixels()**写入象素数据void glDrawPixels(GLsizesi width,GLsizei height,GLenum format,
GLenum type,GLvoid *pixel);
函数参数format和type与glReadPixels()有相同的意义，pixel指向的数组包含所要画的象素数据。
注意，调用这个函数前必须先设置当前光栅位置，若当前光栅位置无效，则给出该函数时不画任何图形，
并且当前光栅位置仍然保持无效。
**glCopyPixels()**
象素拷贝函数是： void glCopyPixels(GLint x,GLint y,GLsizesi width,GLsizei height, GLenum type);
这个函数使用起来有点类似于先调用glReadPixels()函数后再调用 glDrawPixels()一样，但它不需要
将数据写到内存中去，因它只将数据写到framebuffer里。函数功能就是拷贝framebuffer 中左下角点
在(x,y)尺寸为width、height的矩形区域象素数据。数据拷贝到一个新的位置，其左下角点在当前光栅
的位置，参数type可以是GL_COLOR、GL_STENCIL、GL_DEPTH。在拷贝过程中，参数type要按如下方式
转换成format：
```
　　1）若type为GL_DEPTH或GL_STENCIL，那么format应分别是GL_DEPTH_COMPONENT或GL_STENCIL_INDEX；
2）若type为GL_COLOR，format则用GL_RGB或GL_COLOR_INDEX，这要依赖于图形系统是处于RGBA方式还是处于颜色表方式。
```cpp
**glPixelZoom()**一般情况下，图像的一个象素写到屏幕上时也是一个象素，但是有时也需要将图像放大或缩小，OpenGL提供了这个函数：
图像缩放 void glPixelZoom(GLfloat zoomx,GLfloat zoomy);
设置象素写操作沿X和Y方向的放大或缩小因子。缺省情况下，zoomx、zoomy都是1.0。如果它们都是2.0，
则每个图像象素被画到4个屏幕象素上面。**注意**：小数形式的缩放因子和负数因子都是可以的。
**glutSwapBuffers()**
交换缓冲显存
在双缓冲模式下，游戏运行时占用3个图象内存区域。
其中一个是“窗口内存”。其他两个是“缓冲显存1”和“缓冲显存2”。
用glBegin()进行渲染后用glutSwapBuffers，显卡负责把“缓冲显存1”的画面复制到“窗口内存”中。
然后再用glBegin()进行渲染，画面就先渲染到“缓冲显存2”上面了。这时候显卡还在忙着把“缓冲显存1”
上的内容拷贝到“窗口内存”中呢。
所以“渲染过程”和“显示过程”两不误！
```
```cpp
**glutPostRedisplay()**如果是单线程，仅仅用postredisplay而不返回是走不到display的。
应该把这个循环拆开，循环体放在idle里。
**glFrustum()**
glFrustum(left,right,bottom,top,near,far);  参数用来确定视角边界的各个点。
```
![](http://hiphotos.baidu.com/xoxoxo/pic/item/fa9a9325bb89254835a80f20.jpg)
我们的目的就是要通过glFrustum的调用来确定红色区域。首先，图片左侧的圆点就是坐标原点(0,0,0)，
黄色的近平面的左下角坐标（从坐标原点往黄色平面看）用(left, bottom, -near)来指定，而近平面的
右上角坐标则用(right, top,-near)来指定。为了确定红色区域，我们还缺少远平面的坐标点。实际上我
们只需要知道远平面的Z坐标far就可以确定这个平面了。因为远平面的四个角的坐标可以用原点和近平面四
个角的射线和远平面相交来确定。因此我们只需要设定(left,bottom, -near)、(right, top,-near)以
及-far等值就可以确定红色的可视区域了，这也就构成了glFrustum函数的几个参数。
**glBegin()**
OpenGL通过将物体抽象为笛卡尔坐标系下点、线段、多边形的集合，再将点、线段、多边形等通过在函数glBegin()
与glEnd()之间的一系列顶点数据，绘制出图形还原物体。OpenGL通过glBegin()与glEnd()函数完成点、线、三角
形、四边形及多边形的绘制
glBegin(GLenum)
数GLenum有以下10个参数：GL_POINTS：表示将要绘制点
GL_LINES：表示函数开始绘制线
GL_LINE_STRIP：表示函数将开始绘制折线
GL_LINE_LOOP：表示函数将开始绘制闭合曲线
GL_TRIANLES：表示函数开始绘制三角形
GL_TRIANLE_STRIP：表示函数将开始绘制三角形片
GL_TRIANLE_FAN：表示函数将开始绘制三角形扇
GL_QUADS：表示函数开始绘制四边形
GL_QUAD_STRIP：表示函数开始绘制多边形片
GL_POLYGON：表示函数绘制多边形
```cpp
**glRotatef()**物体围绕指定向量旋转指定角度
glRotatef(GLfloat angle,GLfloat x,GLfloat y,GLfloat z)
angle毋庸置疑就是旋转的角度，而x,y,z三个参数则应该看成一个整体表示一个向量，表示物体围绕向量[x,y,z]旋转
```
