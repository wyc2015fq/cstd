# OpenGL ES 3.0（一）绘制第一个三角形 - 我的学习笔记 - CSDN博客





2018年08月07日 22:42:32[zouzhiheng](https://me.csdn.net/u011330638)阅读数：361








# OpenGL 概述

绘制三角形之前，需要对 OpenGL 有一个大致的了解，因此这里引用了 [learnopengl-cn.github.io](https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/) 里的内容：

OpenGL 一般被认为是一个API，然而并不是，OpenGL 仅仅是一个由 Khronos 组织制定并维护的规范。OpenGL 规范严格规定了每个函数该如何执行，以及它们的输出值。至于内部具体每个函数是如何实现的，将由 OpenGL 库的开发者（通常是显卡的生产商）自行决定。当使用新版本的 OpenGL 特性时，只有新一代的显卡能够支持你的应用程序。这也是为什么大多数开发者基于较低版本的 OpenGL 编写程序，并只提供选项启用新版本的特性。

OpenGL 自身是一个巨大的状态机：一系列的变量描述 OpenGL 此刻应当如何运行。假设当我们想告诉 OpenGL 去画线段而不是三角形的时候，我们通过改变一些上下文变量来改变 OpenGL 状态，从而告诉 OpenGL 如何去绘图。一旦我们改变了 OpenGL 的状态为绘制线段，下一个绘制命令就会画出线段而不是三角形。

在 OpenGL 中，任何事物都在 3D 空间中，而屏幕和窗口却是 2D 像素数组，这导致 OpenGL 的大部分工作都是关于把 3D 坐标转变为适应你屏幕的 2D 像素。3D坐标转为2D坐标的处理过程是由 OpenGL 的图形渲染管线（Graphics Pipeline，大多译为管线，实际上指的是一堆原始图形数据途经一个输送管道，期间经过各种变化处理最终出现在屏幕的过程）管理的。图形渲染管线可以被划分为两个主要部分：第一部分把你的3D坐标转换为2D坐标，第二部分是把2D坐标转变为实际的有颜色的像素。

注：2D坐标和像素也是不同的，2D坐标精确表示一个点在2D空间中的位置，而2D像素是这个点的近似值，2D像素受到你的屏幕/窗口分辨率的限制。

图形渲染管线接受一组3D坐标，然后把它们转变为你屏幕上的有色2D像素输出。图形渲染管线可以被划分为几个阶段，每个阶段将会把前一个阶段的输出作为输入。所有这些阶段都是高度专门化的（它们都有一个特定的函数），并且很容易并行执行。正是由于它们具有并行执行的特性，当今大多数显卡都有成千上万的小处理核心，它们在GPU上为每一个（渲染管线）阶段运行各自的小程序，从而在图形渲染管线中快速处理你的数据。这些小程序叫做着色器(Shader)。

有些着色器允许开发者自己配置，这就允许我们用自己写的着色器来替换默认的。这样我们就可以更细致地控制图形渲染管线中的特定部分了，而且因为它们运行在GPU上，所以它们可以给我们节约宝贵的CPU时间。OpenGL着色器是用 OpenGL着色器语言(OpenGL Shading Language, GLSL)写成的。

下面，你会看到一个图形渲染管线的每个阶段的抽象展示。要注意蓝色部分代表的是我们可以注入自定义的着色器的部分。

![图形渲染管线](https://img-blog.csdn.net/20180807224133962?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

首先，我们以数组的形式传递3个3D坐标作为图形渲染管线的输入，用来表示一个三角形，这个数组叫做顶点数据(Vertex Data)；顶点数据是一系列顶点的集合。一个顶点(Vertex)是一个3D坐标的数据的集合。而顶点数据是用顶点属性(Vertex Attribute)表示的，它可以包含任何我们想用的数据，简单起见，可以认为每个顶点只由一个3D位置和一些颜色值组成。

为了让OpenGL知道我们的坐标和颜色值构成的到底是什么，OpenGL需要你去指定这些数据所表示的渲染类型。我们是希望把这些数据渲染成一个点？一个三角形？还是仅仅是一个长长的线？做出的这些提示叫做图元(Primitive)。图元(Primitive) 是一个几何对象，包括点、线、三角形三种类型： 

    1) GL_POINTS，以点的形式进行绘制，通常用在绘制例子效果的场景中 

    2) GL_LINE_STRIP，以线的形式进行绘制，通常用在绘制直线的场景中 

    3) GL_TRIANGLES，以三角形的形式进行绘制，所有二维图像的渲染都会使用这种方式
图形渲染管线的第一个部分是顶点着色器(Vertex Shader)，它把一个单独的顶点作为输入。顶点着色器主要的目的是把3D坐标转为另一种3D坐标（后面会解释），同时顶点着色器允许我们对顶点属性进行一些基本处理。

图元装配(Primitive Assembly)阶段将顶点着色器输出的所有顶点作为输入（如果是GL_POINTS，那么就是一个顶点），并所有的点装配成指定图元的形状。

图元装配阶段的输出会传递给几何着色器(Geometry Shader)。几何着色器把图元形式的一系列顶点的集合作为输入，它可以通过产生新顶点构造出新的（或是其它的）图元来生成其他形状。

几何着色器的输出会被传入光栅化阶段(Rasterization Stage)，这里它会把图元映射为最终屏幕上相应的像素，生成供片段着色器(Fragment Shader)使用的片段(Fragment)。在片段着色器运行之前会执行裁切(Clipping)。裁切会丢弃超出你的视图以外的所有像素，用来提升执行效率。

注：OpenGL中的一个片段是OpenGL渲染一个像素所需的所有数据。

片段着色器(Fragment Shader)的主要目的是计算一个像素的最终颜色，这也是所有OpenGL高级效果产生的地方。通常，片段着色器包含3D场景的数据（比如光照、阴影、光的颜色等等），这些数据可以被用来计算最终像素的颜色。

在所有对应颜色值确定以后，最终的对象将会被传到最后一个阶段，我们叫做Alpha测试和混合(Blending)阶段。这个阶段检测片段的对应的深度（和模板(Stencil)）值，用它们来判断这个像素是其它物体的前面还是后面，决定是否应该丢弃。这个阶段也会检查alpha值（alpha值定义了一个物体的透明度）并对物体进行混合(Blend)。所以，即使在片段着色器中计算出来了一个像素输出的颜色，在渲染多个三角形的时候最后的像素颜色也可能完全不同。

可以看到，图形渲染管线非常复杂，它包含很多可配置的部分。然而，对于大多数场合，我们只需要配置顶点和片段着色器就行了。几何着色器是可选的，通常使用它默认的着色器就行了。

在现代OpenGL中，我们必须定义至少一个顶点着色器和一个片段着色器（因为GPU中没有默认的顶点/片段着色器）。出于这个原因，刚开始学习现代OpenGL的时候可能会非常困难，因为在你能够渲染自己的第一个三角形之前已经需要了解一大堆知识了。

了解了以上知识点之后，就可以尝试着使用 OpenGL 绘制一个三角形了，下面的代码有看不懂的不要紧，具体的语法知识点会在之后的博客中讲解，也可以自行上官网查询各个 API。

# 绘制三角形

OpenGL 程序的编写通常有以下几个步骤： 

    1) 创建着色器 

    2) 编译并加载着色器 

    3) 创建 program 对象并链接着色器 

    4) 设置 viewport 并清理颜色缓冲区 

    5) 绘制图元
接下来将以绘制一个三角形为例进行讲解。

## 创建着色器

#### 顶点着色器

```
#version 300 es
// in 代表该数据由外部程序输入
layout(location=0) in vec4 aColor;
layout(location=1) in vec4 aPosition;
// out 代表该数据将输出到片段着色器中
out vec4 vColor;
void main() {
    vColor = aColor;
    gl_Position = aPosition;
}
```

着色器代码的语法和 C 语言很像，这里先简单介绍一下。

着色器代码是用 OpenGL ES Shading Language(SL) 编写的， 在上面的代码中，第一行指定了 SL 的版本，之后创建了两个数据，分别表示具体数值由外部程序输入，vec4 代表该数据是一个向量，由 4 个标量（即 int、float 等类型）组成。因为像素的最终颜色是由片段着色器最终决定的，所以颜色数据需要输出到片段着色器中，使用 out 标记输出结果。

和 C 语言一样，main() 函数是程序的入口，这里没有作其它处理，只是把外部程序输入的颜色、顶点数据简单赋值给 OpenGL。值得注意的是，gl_Position 是 SL 内置的变量，这个变量定义了将会传送到渲染管线的下一个阶段的 position 值，每一个顶点着色器必须输出一个 position 数据到 gl_Position 变量中。

这一段着色器代码的版本是 OpenGL ES 3.0，如果是 2.0，则版本号为 100(在 OpenGL ES 3.0 中，制定规范的作者决定匹配 OpenGL ES 和 ES SL 的版本，因此直接从 100 跳到了 300)，同时把 layout(location=n) in 改为 attribute，out 改为 varying 即可。

#### 片段着色器

```
#version 300 es
// 指定 float 类型的计算精度
precision mediump float;
// in 代表该数据是由顶点着色器那边传过来的，具体变量名称要一样
in vec4 vColor;
// out 代表该数据将输出到颜色缓冲区中，用于绘制图元
out vec4 fragColor;
void main() {
    fragColor = vColor
}
```

大致上和顶点着色器类似，不同的地方在于，图元的顶点坐标由内置变量 gl_Position 决定，而颜色值由自定义的 out 变量 fragColor 决定。

## 编译并加载着色器

最简单的编写流程为： 

    1) glCreateShader()，创建着色器对象， 

    2) glShaderSource() ，加载着色器源代码到该着色器对象中 

    3) glCompileShader()，编译着色器源代码
在实际编写时，最好检查编译是否成功，完整代码如下：

```cpp
GLuint loadShader(GLenum type, const char *shaderSrc) {
    GLuint shader = glCreateShader(type);
    if (shader == 0) {
        return 0;
    }

    // 加载 shader 源码
    glShaderSource(shader, 1, &shaderSrc, nullptr);

    // 编译
    glCompileShader(shader);

    GLint compiled;
    // 获取编译状态
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) { // 编译失败
        GLint infoLen = 0;
        // 获取输出信息的长度
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char *infoLog = (char *) malloc(sizeof(char) * infoLen);
            // 获取输出信息并打印
            glGetShaderInfoLog(shader, infoLen, nullptr, infoLog);
            LOGE("load %s shader failed: \n%s", type == GL_VERTEX_SHADER ? "vertex" : "fragment",
                 infoLog);
            free(infoLog);
        }

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}
```

## 创建 program 对象并链接着色器

和上一步类似，最简单的编写步骤为： 

    1) glCreateProgram()，创建 program 对象 

    2) glAttachShader()，附着编译好的着色器对象到 program 中 

    3) glLinkProgram() ，链接 program
同样，最好检查链接状态，完整代码如下：

```cpp
GLuint loadProgram(const char *vertexShaderStr, const char *fragmentShaderStr) {
    GLuint program = glCreateProgram();
    if (program == 0) {
        LOGE("create program failed");
        return 0;
    }

    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderStr);
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderStr);
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint infoLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char *infoLog = (char *) malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(program, infoLen, nullptr, infoLog);
            LOGE("loadProgram failed: %s", infoLog);
            free(infoLog);
        }

        glDeleteProgram(program);
        return 0;
    }

    return program;
}
```

## 设置 viewport 并清理颜色缓冲区

设置 viewport：
`glViewport(0, 0, mWidth, mHeight);`
glViewport 这个函数会通知 OpenGL ES 要绘制的 2D 渲染表面（rendering surface）的原点、宽高，相当于定义了一个画布，作为 OpenGL ES 的渲染区域。

设置了 viewport 之后，下一步需要清理屏幕，在 OpenGL ES 中，绘图涉及的缓冲区有多种类型：颜色缓冲区、深度缓冲区、模板 （stencil）缓冲区，在本例中，只有颜色缓冲区会被绘制。在绘制每一帧之前，需要使用 glClear 函数清理颜色缓冲区：
`glClear(GL_COLOR_BUFFER_BIT);`
缓冲区将被清除为 glClearColor 所指定的颜色，清除颜色应该在 glClear 作用于颜色缓冲区之前设置好：

```cpp
glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
```

## 绘制图元

首先需要创建顶点、颜色数组，用于输入到着色器中：

```cpp
// 使用 (x, y, z) 表示一个顶点坐标，因此该顶点矩阵共描述了三个顶点
const static GLfloat VERTICES[] = {
        -0.5f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f
};

// 分别表示 r, g, b, a
const static GLfloat COLORS[] = {
        0.0f, 1.0f, 1.0f, 1.0f
};

static const GLuint ATTRIB_COLOR = 0; // 对应 layout(location=0) in vec4 aColor
static const GLuint ATTRIB_POSITION = 1; // 对应 layout(location=1) in vec4 aPosition
static const GLsizei VERTEX_COUNT = 3; // 顶点数量
```

注意 OpenGL 的坐标系长这样：

![OpenGL 坐标系](https://img-blog.csdn.net/20180807225313122?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

即 OpenGL 坐标轴的范围为 [-1, 1]，也可以按照手机屏幕定义三角形的顶点坐标，再等比例缩小到 [-1, 1] 的范围，这个过程就叫做标准化（normalized）。

顶点数组定义好之后就可以绘制图元了：

```
// 设置颜色属性数据
glVertexAttrib4fv(ATTRIB_COLOR, COLORS);

// 设置顶点数据数据
glVertexAttribPointer(ATTRIB_POSITION, VERTEX_COUNT, GL_FLOAT, GL_FALSE, 0, VERTICES);

// 启用顶点属性数组
glEnableVertexAttribArray(ATTRIB_POSITION);
// 绘制图元
glDrawArrays(GL_TRIANGLES, 0, VERTEX_COUNT);
// 禁用顶点属性数组
glDisableVertexAttribArray(ATTRIB_POSITION);
```

如此，就完成了一个三角形的绘制，完整代码见 [GitHub](https://github.com/zouzhiheng/AVGraphics)。







