# OpenGL ES 3.0（五）纹理 - 我的学习笔记 - CSDN博客





2018年08月11日 22:12:37[zouzhiheng](https://me.csdn.net/u011330638)阅读数：80








我们可以为每个顶点添加颜色来增加图形的细节，从而创建出有趣的图像。但是，如果想让图形看起来更真实，我们就必须有足够多的顶点，从而指定足够多的颜色。这将会产生很多额外开销，因为每个模型都会需求更多的顶点，每个顶点又需求一个颜色属性。因此，一般情况下会使用纹理达到相同的效果。

纹理可以认为是一张可以贴到物体（图元）表面上的图片/花纹/贴纸，使物体（图元）变得生动、精细。

# 纹理的使用

## 创建并设置纹理

```
// 纹理对象的句柄
GLuint textureId;

// 一张宽高为 2 x 2 的图片, 每个像素用 (R, G, B) 表示，占 3 bytes
GLubyte pixels[4 * 3] =
{
    255, 0, 0,  // Red
    0, 255, 0,  // Green
    0, 0, 255,  // Blue
    255, 255, 0 // Yellow
};

// 指定像素的对其方式
glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

// 生成一个纹理对象
glGenTextures(1, &textureId);

// 绑定该纹理对象到 GL_TEXTURE_2D 中
glBindTexture(GL_TEXTURE_2D, textureId);

// 加载纹理对象并填充像素数据
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

// 设置纹理参数
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
```

## 片段着色器

在着色器代码中，使用 sampler 代表一个纹理，比如 2D 纹理，就是 sampler2D。

```
#version 300 es

precision mediump float;

uniform sampler2D sTexture;

in vec2 vTexCoord;

layout(location=0) out vec4 fragColor;

void main() {
    fragColor = texture(sTexture,  vTexCoord);
}
```

vTexCoord 是从顶点着色器传过来的坐标矩阵，自定义一个就好，比如铺满屏幕的坐标矩阵如下：

```cpp
static GLfloat TEXTURE_COORDS[] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
};
```

和 OpenGL [-1, 1] 的坐标系不同，纹理的坐标系如下：

![纹理坐标](https://img-blog.csdn.net/20180825100041992?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 绘制纹理

```
glViewport(0, 0, mWidth, mHeight);
    ...

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUniform1i(textureLoc, 0); // 0 代表 GL_TEXTURE0

    ... // 填充顶点坐标

    glDrawArrays(...); // 绘制
```

textureLoc 是使用 glGetUniformLocation(program, “sTexture”) 得到的。

# 函数详解

## 使用纹理对象

```
void glGenTextures(GLsizei n, GLuint *textures);

void glBindTexture(GLenum target, GLuint texture);

void glDeleteTextures(GLsizei n, GLuint *textures);
```

## glPixelStorei

OpenGL 默认采用四字节的对齐方式，因此存储一个图像所需的字节数并不完全等于 “宽*高*像素字节数” ，比如，图像的宽度为 199，包含 RGB 3 个颜色分量，如果硬件结构是 4 字节排列的话，此时一行图像需要的存储空间就不等于：199*3=597，而应该在末尾填充 3 个空字节，因此是 600 字节。可以使用下列函数改变像素的存储方式：

```
void glPixelStorei(GLenum pname, GLint param);
void glPixelStoref(GLenum pname, GLfloat param);
```

比如：
`glPixelStorei(GL_UNPACK_ALIGNMENT, 1);`
其中 GL_UNPACK_ALIGNMENT 指定 OpenGL 如何从数据缓冲区中解包图像数据。

## glTexImage2D

从缓冲区中载入数据：

```
// level: mip 贴图层次（mip 贴图稍后介绍）
void glTexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, void *data);

void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, void *data);

void glTexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, void *data);
```

width、height、depth 最好使用 2 的次幂，以获取更高的性能。

## glTexParameterf

```
// target: GL_TEXTURE_2D、GL_TEXTURE_3D 等
// pname: 参数名
// param: 参数值
void glTexParameterf(GLenum target, GLenum pname, GLfloat param);
void glTexParameteri(GLenum target, GLenum pname, GLint param);
void glTexParameterfv(GLenum target, GLenum pname, GLfloat *params);
void glTexParameteriv(GLenum target, GLenum pname, GLint *params);
```

### 基本过滤

纹理图像中的纹理单元和屏幕上的像素几乎从来不会形成一对一的对应关系，因此，当纹理图像应用于几乎图形的表面时，纹理图像不是被拉伸就是被收缩。根据一个被拉伸/被收缩的纹理贴图计算颜色片段的过程称为纹理过滤，可以同时设置放大和缩小过滤器，即 GL_TEXTURE_MAG_FILTER 和 GL_TEXTURE_MIN_FILTER，取值为 GL_NEAREST 或 GL_LINEAR，分别对应最邻近过滤和线性过滤。确保总是为 GL_TEXTURE_MIN_FILTER 选择这两种过滤器的一种，因为默认的过滤器不适用于 mip 贴图（参考下面）。

最邻近过滤是最简单、最快速的过滤方法，最显著的特性是当纹理被拉伸到特别大时所出现的大片斑驳状像素。

线性过滤不是把最邻近的纹理单元应用到纹理中，而是把这个纹理周围的纹理单元的加权平均值应用到这个纹理坐标上。线性过滤最显著的特征是当纹理被拉伸时所出现的“失真”图形。

![纹理过滤](https://img-blog.csdn.net/20180825095137770?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

和最邻近过滤相比，线性过滤会带来一些额外的开销，但在如今的高速硬件上，相比它所实现的效果，这些开销几乎可以忽略不计。

### 纹理环绕

正常情况下，纹理坐标范围为 0.0 ~ 1.0，如果坐标在这个范围之外，OpenGL 则根据纹理环绕模式处理这个问题，可设置参数有 GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, 或 GL_TEXTURE_WRAP_R，可选值有：GL_REPEAT, GL_CLAMP, GL_CLAMP_TO_EDGE, 或 GL_CLAMP_TO_BORDER。
|环绕方式|描述|
|----|----|
|GL_REPEAT|对纹理的默认行为，重复纹理图像|
|GL_MIRRORED_REPEAT|和GL_REPEAT一样，但每次重复图片是镜像放置的|
|GL_CLAMP_TO_EDGE|纹理坐标会被约束在 0 到 1 之间，超出的部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果|
|GL_CLAMP_TO_BORDER|超出的坐标为用户指定的边缘颜色|

![纹理环绕](https://img-blog.csdn.net/2018082509530982?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# Mip 贴图

Mip 贴图是一种强大的纹理技巧，不仅可以提高渲染性能，还可以改善场景的显示质量。常用于处理两个问题：

1) 闪烁（锯齿假影），当被渲染物体的表面和它所应用的纹理图像相比显得非常小时，就会出现这个问题

2) 性能，必须加载大量的纹理内存对纹理图像进行过滤处理，但屏幕上时机显示的只是很少的一部分片段

使用 Mip 贴图，本质上，不是把单个图像加载到纹理状态中，而是把一系列从最大到最小的图像加载到单个“Mip贴图”纹理状态，然后，OpenGL 使用一组新的过滤模式，为一个特定的几何图形选择具有最佳过滤效果的纹理，在付出一些额外内存的代价之后，不进可以消除闪烁效果，还可以大大降低对远处物体进行纹理贴图时所需要的内存及处理开销，并且还可以维护一组具有更高分辨率的可用纹理。

Mip 贴图纹理由一系列纹理图像组成，每个图像大小在每个轴的方向上都缩小一半，即为原来的 1/4，知道最后一个图像大小是 1x1 为止（一个轴上的大小到达 1 时，不会再减半），使用一组正方形的 Mip 贴图所要求的内存比不使用 Mip 贴图要多出三分之一。

![Mip 贴图纹理](https://img-blog.csdn.net/20180811220929253?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

Mip 贴图层是通过 glTexImage 函数加载的，level 参数指定了图像数据用于哪个 Mip 层，第一层为 0，如果 Mip 贴图未被使用，那么就只有第 0 层会被加载，默认情况下，所有的 Mip 层都必须加载，但可以设置需要使用的基层和最大层：

```
// 只加载第 0 层至第 4 层
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
```

还可以使用 GL_TEXTURE_MIN_LOD、GL_TEXTURE_MAX_LOD 限制已加载的 Mip 层的使用范围。

# 纹理压缩

用于减小存储纹理图像所需的内存。压缩格式有：
|Compressed Format|Base Internal Format|
|----|----|
|GL_COMPRESSED_RGB|GL_RGB|
|GL_COMPRESSED_RGBA|GL_RGBA|
|GL_COMPRESSED_SRGB|GL_RGB|
|GL_COMPRESSED_SRGB_ALPHA|GL_RGBA|
|GL_COMPRESSED_RED|GL_RED|
|GL_COMPRESSED_RG|GL_RG (Red Green)|

判断纹理是否被成功压缩：

```
GLint compFlag;
. . .
// 成功返回 1，否则返回 0
glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &compFlag);
```

使用 glHint 指定压缩算法（最快速度或最佳质量）：

```
glHint(GL_TEXTURE_COMPRESSION_HINT, GL_FASTEST);
glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);
glHint(GL_TEXTURE_COMPRESSION_HINT, GL_DONT_CARE);
```

可以使用原始压缩数据，从而极大地提到纹理的加载速度（这个技巧只有在完全遵循标准的硬件实现中才是可行的）：

```
void glCompressedTexImage1D(GLenum target, GLint level, GLenum internalFormat,
GLsizei width, GLint border, GLsizei imageSize, void *data);

void glCompressedTexImage2D(GLenum target, GLint level, GLenum internalFormat,
GLsizei width, GLsizei height, GLint border, GLsizei imageSize, void *data);

void glCompressedTexImage3D(GLenum target, GLint level, GLenum internalFormat,
GLsizei width, GLsizei height, GLsizei depth, GLint border, Glsizei imageSize, GLvoid *data);
```

对于有些纹理，压缩可能会导致图像质量的损失，而具有大量细节的纹理在视觉上几乎与未压缩的源纹理相同，纹理压缩方法的选择可能极大地依赖于底层图像的本质。



