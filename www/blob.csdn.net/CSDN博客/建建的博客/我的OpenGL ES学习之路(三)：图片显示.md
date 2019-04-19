# 我的OpenGL ES学习之路(三)：图片显示 - 建建的博客 - CSDN博客
2018年03月05日 18:12:22[纪建](https://me.csdn.net/u013898698)阅读数：419

这次的任务是把一张图片用`OpenGL ES`的方式显示到屏幕上，部分功能使用了`GLKit`库。
### 渲染上下文
先来看一下程序中定义的属性：
![](https://upload-images.jianshu.io/upload_images/1373592-513fb5015b015122.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/487)
定义的属性.png
`EGL`连接了`OpenGL ES`与本地原生窗口（例如iOS系统）。Apple提供了自己的EGL的API，就是EGAL，`EAGLContext`就是属于`EGAL`。
`EAGLContext`是渲染上下文，OpenGL ES 必须有一个可用的上下文才能绘图，因为一个应用程序可能创建多个EAGLContext，所以我们需要关联特定的`EAGLContext`和`渲染表面`，这一步称为“`设置当前上下文`”
创建EAGLContext，创建的时候需要指定`EAGLRenderingAPI`类型，`EAGLRenderingAPI`取值类型有：`kEAGLRenderingAPIOpenGLES1`，`kEAGLRenderingAPIOpenGLES2`，`kEAGLRenderingAPIOpenGLES3`，分别对应着`OpenGL ES 1.0`，`OpenGL ES 2.0`，`OpenGL ES 3.0`，这里我们使用2.0。
![](https://upload-images.jianshu.io/upload_images/1373592-ffa04d344015d7ac.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/694)
创建EAGLContext
![](https://upload-images.jianshu.io/upload_images/1373592-d4996030534e5421.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/421)
设置当前上下文
设置controller的当前上下文是刚才创建的EAGLContext，设置颜色缓冲区的格式为`RGBA8888`的格式
![](https://upload-images.jianshu.io/upload_images/1373592-22b5d3feb25a97b9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/574)
设置当前view的上下文和颜色缓冲区格式
### “顶点坐标” 和 “顶点属性”
定义要绘制图片的`顶点坐标`和`纹理坐标`
OpenGL ES中坐标系是和iOS常用的`Quartz 2D坐标系`是不一样的，Quartz 2D坐标系属于`右手坐标系`，OpenGL ES属于`右手坐标系`。先说一下左手坐标系和右手坐标系。
伸出左手，让拇指和食指成“L”形状，拇指向右，食指向上，中指指向起那方，这时就建立了一个“`左手坐标系`”，拇指、食指和中指分表代表x、y、z轴的正方向。“`右手坐标系`”就是用右手，如下图所示：
![](https://upload-images.jianshu.io/upload_images/1373592-9b6a1a7a4ef37935.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/682)
“左手坐标系”和“右手坐标系”
在iOS开发中，屏幕左上角是坐标原点，往右是x轴正方向，往下是y轴正方向。而在OpenGL ES中，屏幕的中点是坐标原点，往右是x轴正方向，往下是y轴正方向，其中z轴的正方向是从屏幕往外的方向，如下图所示：
![](https://upload-images.jianshu.io/upload_images/1373592-d554f13f8718c7a5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/376)
OpenGL ES坐标系
根据OpenGL ES的坐标系，我们定义一下要绘制的图片的几个顶点，顶点坐标和纹理坐标是放在一个`GLfloat数组`中管理的，定义一组顶点数据的跨度为5，其中前三个存储`顶点坐标`，后两个存储`纹理坐标`，下图一共定义了4个顶点，就是矩形的四个顶点，需要注意的是，虽然坐标都是0.5，但是绘制出来的图形并不是正方形，因为我们用来最终显示的是iPhone屏幕，手机的长和宽并不相等。
![](https://upload-images.jianshu.io/upload_images/1373592-b33b485d034bfcd1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/422)
顶点坐标和纹理坐标
OpenGL ES不能绘制多边形，只能绘制`点`，`线`，`三角形`，OpenGL可以绘制多边形，由于我们绘制的图片是一个矩形，又两个三角形构成，就是下图中的两个`顶点索引`（0，1，3）和（1，2，3）组成的三角形拼成一个矩形
![](https://upload-images.jianshu.io/upload_images/1373592-694833d22f0a4aa8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/411)
顶点索引
根据顶点索引的个数，计算要绘制的顶点的个数
![](https://upload-images.jianshu.io/upload_images/1373592-2d788e207401ae16.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/469)
顶点数量
## “顶点缓存” 和 “顶点索引缓存”
程序会保存3D场景数据到RAM中，CPU有专门为其分配的RAM，在图形处理的过程中，GPU也会专门为其分配RAM。使用硬件渲染3D图形的速度几乎完全取决于不同的内存区域的访问的方式。
OpenGL ES部分运行在CPU上，部分运行在GPU上。OpenGL ES横跨在两个处理器之间，协调两个内存区域之间的数据交换，下图表示3D渲染相关的组件之间的数据交换，每个箭头都代表着一个渲染性能的瓶颈。
![](https://upload-images.jianshu.io/upload_images/1373592-cafd74dbfe1f9100.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/401)
硬件组件和OpenGL ES之间的关系
OpenGL ES通常会高效地协调数据交换，但是程序与OpenGL ES的交互方式会增加交换的数量和类型，从一个内存区域复制数据到内外一个内存区域，速度是相对比较慢的，另外，在发生内存复制的时候，这两块内存都不能用作它用，因此内存区域之间的数据交换尽量避免。最新的嵌入式CPU可以很容易的完成以亿为单位的运算，但是内存读写只能在百万单位，这意味着，除非CPU能够在每次从内存读取一块数据后有效的运行5个或者更多的运算，否则处理器的性能就处于次优的状态，也叫`数据饥饿`，这种情况对于GPU来说更明显。
OpenGL ES为了解决这个问题，定义了`缓存（buffer）`，为缓存提供数据需要以下几个步骤：
> - `生成 (Generate)`：请求OpenGL ES为buffer生成一个独一无二的标识符
- `绑定 (Bind)`：告诉OpenGL ES为接下来的运算使用一个buffer
- `缓存数据 (Buffer Data)`：为当前绑定的buffer分配并初始化足够的内存，从cpu控制的内存复制数据到buffer
- `启用 (Enable) 或者禁止 (Disable)`：告诉OpenGL ES在接下来的渲染中是否使用缓存中的数据
- `设置指针 (Pointer)`：告诉OpenGL ES在buffer中的数据的类型和所需要访问的数据的内存偏移值。
- `绘图 (Draw)`：告诉OpenGL ES使用当前绑定并启用的buffer中的数据，来渲染场景
- `删除 (Delete)`：告诉OpenGL ES删除以前生成的buffer并释放相关的buffer
上面的几个步骤分别对应着下面的几个OpenGL ES的API：
```cpp
```cpp
/* n: 要申请的缓冲区对象数量
   buffer: 指向n个缓冲区的数组指针，该数组存放的是缓冲区的名称
   返回的缓冲区对象名称是0以外的无符号整数，0是OpenGL ES的保留值，不表示具体的缓冲区对象，修改或者查询0的缓冲区状态产生错误
*/
void glGenBuffers(GLsizei n, GLuint *buffer);
```
```
```cpp
```cpp
target：用于指定当前的缓冲区对象的"类型"
           GL_ARRAY_BUFFER：数组缓冲区
           GL_ELEMENT_ARRAY_BUFFER：元素数组缓冲区
           GL_COPY_READ_BUFFER：复制读缓冲区
           GL_COPY_WRITE_BUFFER：复制写缓冲区
           GL_PIXEL_PACK_BUFFER：像素包装缓冲区
           GL_PIXEL_UNPACK_BUFFER：像素解包缓冲区
           GL_TRANSFORM_FEEDBACK_BUFFER：变换反馈缓冲区
           GL_UNIFORM_BUFFER：统一变量缓冲区
 buffer: 缓冲区的名称
void glBindBuffer(GLenum target, GLuint buffer);
```
```
OpenGL ES使用`数组缓冲区`和`元素数组缓冲区`两种缓冲区类型分别指定`顶点`和`图元数据`，`GL_ARRAY_BUFFER`类型用于创建保存**顶点数据**的缓冲区对象，`GL_ELEMENT_ARRAY_BUFFER`用于创建保存**图元索引**的缓冲区对象。
需要注意的是，在用`glBindBuffer`绑定之前，分配缓冲区并不一定非得用glGenBuffers，可以指定一个未使用的缓冲区对象。但是为了避免不必要的错误，还是建议使用glGenBuffers让系统给我们分配未使用的缓冲区对象的名称
```cpp
```cpp
/* target: 用于指定当前的缓冲区对象的"类型"
   size: 缓冲区数据存储大小，以字节表示
   data: 缓冲区数据的指针
   usage: 应用程序将如何使用缓冲区对象中存储的数据的提示，也就是缓冲区的使用方法，初始值为 GL_STATIC_DRAW
*/
void glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
```
```
缓冲区的使用方法取值有很多种，这里我们使用GL_STATIC_DRAW，这个值的意思是缓冲区对象将被修改一次，使用多次，以绘制图元或指定的图像。因为我们之后的操作不对其进行修改，只是初始化的时候赋值一次。这个取值可以帮助OpenGL ES优化内存的使用
如果使用GL_DYNAMIC_DRAW，意义是缓冲区对象将被重复修改，使用多次。这会提示上下文，缓存内的数据会频繁改变，OpenGL ES就会以不同的方式来处理缓存的存储。
回到当前的例子中，具体代码如下：
![](https://upload-images.jianshu.io/upload_images/1373592-69b005663f1f0d7b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
顶点数据和顶点索引的缓存
## “启用顶点数组” 和 “指定顶点属性”
先来说一下顶点属性，顶点数据也叫顶点属性，指定每个顶点的数据，每个顶点的数据可以每个顶点挨个设置，就是`顶点数组`，也可以用一个常量设置于所有的顶点，就是`常量顶点属性`。例如，绘制一个红色的三角形，可以指定一个常量顶点属性来设置三角形的全部3个顶点，但是组成三角形的3个顶点的位置坐标不同，可以使用顶点数组来指定。
我们上面已经定义了顶点数组：
//启用顶点位置(坐标)数组，之前说过opengl是状态机，需要什么状态就启动什么状态
glEnableVertexAttribArray(GLKVertexAttribPosition);
```cpp
```cpp
GLfloat vertexs[] = {
        -0.5, -0.5, 0,     0.0, 0.0,   //左下
        -0.5,  0.5, 0,     0.0, 1.0,   //左上
         0.5,  0.5, 0,     1.0, 1.0,   //右上
         0.5, -0.5, 0,     1.0, 0.0,   //右下
    };
```
```
##### <a name="fenced-code-block">启用通用顶点属性</a>
```cpp
```cpp
/*
  index：指定通用顶点数据的索引，这个值的范围从0到支持的最大顶点属性数量减1
  功能：用于启用通用顶点属性
*/
void glEnableVertexAttribArray(GLuint index);
```
```
##### <a name="fenced-code-block">禁止通用顶点属性</a>
```cpp
```cpp
/*
  index：指定通用顶点数据的索引，这个值的范围从0到支持的最大顶点属性数量减1
*/
void glDisableVertexAttribArray(GLuint index);
```
```
##### <a name="fenced-code-block">常量顶点属性设置值</a>
```cpp
```cpp
// 加载index指定的通用顶点属性。
// 下面的API中没有的值默认为1.0，比如glVertexAttrib1f/v设置的值为(x, 1.0, 1.0, 1.0)
void glVertexAttrib1f(GLuint index, GLfloat x);
void glVertexAttrib2f(GLuint index, GLfloat x, GLfloat y);
void glVertexAttrib3f(GLuint index, GLfloat x, GLfloat y, GLfloat z);
void glVertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void glVertexAttrib1fv(GLuint index, const GLfloat *values);
void glVertexAttrib2fv(GLuint index, const GLfloat *values);
void glVertexAttrib3fv(GLuint index, const GLfloat *values);
void glVertexAttrib4fv(GLuint index, const GLfloat *values);
```
```
##### <a name="fenced-code-block">顶点数组设置值</a>
```cpp
```cpp
index: 通用顶点属性索引
size: 顶点数组中为顶点属性指定的分量数量，取值范围1～4
type: 数据格式 ，两个函数都包括的有效值是
      GL_BYTE  GL_UNSIGNED_BYTE  GL_SHORT  GL_UNSIGNED_SHORT  GL_INT  GL_UNSIGNED_INT
      glVertexAttribPointer还包括的值为：GL_HALF_FLOAT GL_FLOAT 等
normalized: 仅glVertexAttribPointer使用，表示非浮点数据类型转换成浮点值时是否应该规范化
stride: 每个顶点由size指定的顶点属性分量顺序存储。stride指定顶点索引i和i+1表示的顶点之间的偏移。
    如果为0，表示顺序存储。如果不为0，在取下一个顶点的同类数据时，需要加上偏移。
ptr: 如果使用“顶点缓冲区对象”，表示的是该缓冲区内的偏移量。
void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *ptr);
// 取值为“整数”版本
void glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *ptr);
```
```
> 
**需要注意的是**：上面的几个API中的index参数，对应着顶点着色器中的响应变量的位置，可以使用着色器语言GLSL的修饰符来表示：
`layout(location = 0) in vec4 a_color; layout(location = 1) in vec4 a_position;`
设置颜色的属性的时候，可以使用index=0；使用顶点坐标的时候可以设置index=1
或者
使用`glBindAttribLocation`函数来设置，具体的以后再讲。
这里由于我们使用iOS封装好的GLkit框架，不需要我们设置着色器程序，里面有内置的设置好的index位置，就是下面的变量：
```
typedef NS_ENUM(GLint, GLKVertexAttrib)
{
    GLKVertexAttribPosition,
    GLKVertexAttribNormal,
    GLKVertexAttribColor,
    GLKVertexAttribTexCoord0,
    GLKVertexAttribTexCoord1
} NS_ENUM_AVAILABLE(10_8, 5_0);
```
在回到我们的程序，我们使用GLKit里面的`GLKVertexAttribPosition`和`GLKVertexAttribTexCoord0`分别表示`顶点坐标`和`纹理坐标`两个变量的属性索引。(顶点索引不是顶点数据，这里我们不需要管这个值)
![](https://upload-images.jianshu.io/upload_images/1373592-51ac83ba3ad4ebc1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
启用顶点数组和指定顶点属性
- 上面的程序最需要注意的是变量的偏移很重要，我由于错把GLfloat写成CGFloat，导致图片怎么都渲染不出来。
### 设置纹理贴图
我们把一张图片加载成为要渲染的纹理，由于纹理坐标系是跟手机显示的Quartz 2D坐标系的y轴正好相反，纹理坐标系使用左下角为原点，往上为y轴的正值，往右是x轴的正值，所以需要设置一下`GLKTextureLoaderOriginBottomLeft`。
GLKit中使用`GLKTextureInfo`表示纹理对象。
![](https://upload-images.jianshu.io/upload_images/1373592-0f1599d0afaba237.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
纹理
### 着色器
GLKit提供的`GLKBaseEffect`是对OpenGL ES中的着色器的封装。
下面的代码创建GLKBaseEffect，并且把GLKBaseEffect的纹理功能打开，然后将`GLKTextureInfo`赋值给`GLKBaseEffect`的纹理
![](https://upload-images.jianshu.io/upload_images/1373592-43e65a01a90b5eaa.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/421)
GLKBaseEffect
### 渲染
GLKit提供了`GLKViewDelegate`，`GLKView`里面有个delegate属性，我们需要实现这个协议。这个协议的方法的刷新频率和屏幕的刷新频率是一致的，在- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect这个方法中进行渲染操作.
![](https://upload-images.jianshu.io/upload_images/1373592-bbe760dca72a7828.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
GLKViewDelegate
在渲染操作中，我们来看几个API：
OpenGL ES是一个交互式的渲染系统，在每一帧的开始，将缓冲区的所有内容初始化为默认镇。如果想把值一开始统一设置为一个值，缓冲区可以通过`glClear`函数清除，用一个掩码位来表示应该清除为其指定值的各个缓冲区
```cpp
```cpp
// mask: 指定要清除的缓冲区，由下面几个表示各种OpenGL ES缓冲区的位掩码联合组成：
    GL_COLOR_BUFFER_BIT
    GL_DEPTH_BUFFER_BIT
    GL_STENCIL_BUFFER_BIT
void glClear(GLbitfield mask);
```
```
设置清除为哪个默认值，可以通过下面的函数来设置，下面的几个函数分别对应着上面的几个掩码
```cpp
```cpp
// GL_COLOR_BUFFER_BIT 颜色
void glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat  alpha);
// GL_DEPTH_BUFFER_BIT 深度
void glClearDepthf(GLfloat depthf);
// GL_STENCIL_BUFFER_BIT 模版
void glClearStencil(GLint s);
```
```
```cpp
```cpp
/* mode: 指定要绘制的图元，我们绘制两个三角形，这里用GL_TRIANGLES
count: 要绘制的“顶点数量”
type：指定的顶点索引的存储的值的类型
indices: 指向顶点索引的数组指针。*/
void glDrawElements(GLenum mode, GLsizei count, GLEnum type, const GLvoid *indices);
```
```
![](https://upload-images.jianshu.io/upload_images/1373592-d9d7affc1f01fbc2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/513)
渲染操作
### 图片显示
不容易，经过上面这么多步骤，我们终于把一张图片显示到了手机屏幕上，来看一下结果。然而，这仅仅是OpenGL ES万里长征的第一步，继续加油吧！
![](https://upload-images.jianshu.io/upload_images/1373592-d39b80b95fc6690f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/374)
渲染结果
作者：o0阿拉斯加的狗0o
链接：https://www.jianshu.com/p/954339d57541
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
