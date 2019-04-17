# OpenGL ES 3.0（六）缓冲区对象、PBO、FBO - 我的学习笔记 - CSDN博客





2018年08月18日 11:14:13[zouzhiheng](https://me.csdn.net/u011330638)阅读数：341








# 缓冲区对象

创建：

```
Gluint pixBuffObjs[1];
glGenBuffers(1, pixBuffObjs);
```

绑定：
`glBindBuffer(GL_PIXEL_PACK_BUFFER, pixBuffObjs[0]);`
缓冲区对象的绑定点有：
|Target Name|Description|
|----|----|
|GL_ARRAY_BUFFER|用于存储顶点属性，比如颜色、位置、纹理坐标等|
|GL_COPY_READ_BUFFER|glCopyBufferSubData 的数据源|
|GL_COPY_WRITE_BUFFER|glCopyBufferSubData 的目标|
|GL_ELEMENT_ARRAY_BUFFER|用于保存 glDrawElements, glDrawRangeElements, and glDrawElementsInstanced 的索引|
|GL_PIXEL_PACK_BUFFER|读取像素数据的源缓冲区（比如 glReadPixels）|
|GL_PIXEL_UNPACK_BUFFER|纹理更新的源缓冲区，比如 glTexImage1D, glTexImage2D, glTexImage3D, glTexSubImage1D, glTexSubImage2D, and glTexSubImage3D|
|GL_TEXTURE_BUFFER|Buffer accessible to shaders through texel fetches|
|GL_TRANSFORM_FEEDBACK_BUFFER|Buffer written to by a transform feedback vertex shader|
|GL_UNIFORM_BUFFER|Uniform values accessible to shaders|

删除：
`glDeleteBuffers(1, pixBuffObjs);`
填充缓冲区:
`glBufferData(GL_PIXEL_PACK_BUFFER, pixelDataSize, pixelData, GL_DYNAMIC_COPY);`
在使用 glBufferData 之前，必须先进行绑定。如果要分配一个特定大小的缓冲区，而不需要对它进行填充，则 pixelData 可以为 NULL。第四个参数可能的值为：
|Buffer Usage|Description|
|----|----|
|GL_STREAM_DRAW|缓冲区内容将由应用程序设置一次，并且不经常用于绘图|
|GL_STREAM_READ|缓冲区内容将被设置一次，作为 OpenGL 的输出，并且不经常用于绘图|
|GL_STREAM_COPY|缓冲区内容将被设置一次，作为 OpenGL 的输出，并且不经常用于绘制或复制到其他图像|
|GL_STATIC_DRAW|缓冲区内容将由应用程序设置一次，并经常用于绘图或复制到其他图像|
|GL_STATIC_READ|缓冲区内容将被设置一次，作为 OpenGL 的输出，并被应用程序多次查询|
|GL_STATIC_COPY|缓冲区内容将被设置一次，作为 OpenGL 的输出，并经常用于绘制或复制到其他图像|
|GL_DYNAMIC_DRAW|缓冲区内容将由应用程序频繁更新，并经常用于绘制或复制到其他图像|
|GL_DYNAMIC_READ|缓冲区内容将作为 OpenGL 的输出频繁更新，并由应用程序多次查询|
|GL_DYNAMIC_COPY|缓冲区内容将作为 OpenGL 的输出频繁更新，并经常用于绘图或复制到其他图像|

# PBO

像素缓冲区是一个很好的容器，能够暂时存储 GPU 本地像素数据。

像素缓冲区绑定点有两个：

1) GL_PIXEL_PACK_BUFFER，当一个像素缓冲区绑定到这个目标上时，任何读取像素的 OpenGL 操作都会从像素缓冲区中获得数据，包括 glReadPixels, glGenTexImage 和 glGetCompressedImage。同时像素数据在 GPU 内存中的像素缓冲区对象中的任务就结束了，而并不会下载到客户端。

2) GL_PIXEL_UNPACK_BUFFER，当一个像素缓冲区绑定到这个目标上时，读取操作会成为 GPU 内存中的 PBO，同时任何绘制像素的 OpenGL 操作都会写入该像素缓冲区，包括 glTexImage*D, glTexSubImage*D, glCompressedImage*D 和 glCompressedSubImage*D，这些操作将数据和纹理从本地 CPU 内存中读取到帧缓冲区中。

像素缓冲区的作用：任何从 PBO 的读取或写入 PBO 的操作或任何缓冲区对象都用管线进行处理，这意味着 GPU 不需要完成所有其他操作，提高效率。

缓冲区对象在处理需要经常访问、修改或更新数据的场合中具有重大优势，比如异步调用 glReadPixels。将像素数据读取到 CPU 内存中需要 GPU 先完成正在进行的其它所有工作，如果 GPU 忙于渲染 3D 图形，这将影响图形渲染的效率。这时候可以使用 PBO，因为读取操作是通过管线进行的，所有对 glReadPixels 的调用能够立即返回。甚至可以多次调用，使用不同的缓冲区对象来读取不同的区域。

使用任何缓冲区对象都需要先分配存储空间：
`glBufferData(GL_PIXEL_PACK_BUFFER, pixelDataSize, pixelData, GL_DYNAMIC_COPY);`
pixelData 可以设为 NULL，如果计划为多种不同大小的数据使用 PBO，最好关闭数据大小上限的设定。

所有对 glBufferData 的调用和其它绘制操作都通过管线进行处理，这意味着 OpenGL 实现不需要等待所有活动停止，就可以将数据发送给 GPU。

读取像素数据（从当前启用的读取缓冲区的特定位置获取像素，然后将它们复制到 CPU 内存中）：

```
void* data = (void*)malloc(pixelDataSize);
glReadBuffer(GL_BACK_LEFT);
glReadPixels(0, 0, GetWidth(), GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, pixelData);
```

当我们向客户端（CPU）内存进行写入时，整个管线经常需要被清空，以保证读取工作能够完成，这对应用程序的性能有很大的影响。可以使用缓冲区对象来克服这个问题，在调用 glReadPixels 并将 glReadPixels 调用中的数据指针设为 1 之前，可以将一个缓冲区对象绑定到 GL_PIXEL_PACK_BUFFER 上，这样就可以将像素重定向到 GPU 中的一个缓冲区中，避免了复制到客户端内存上可能带来的性能问题：

```
glReadBuffer(GL_BACK_LEFT);
glBindBuffer(GL_PIXEL_PACK_BUFFER, pixBuffObjs[0]);
glReadPixels(0, 0, GetWidth(), GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, NULL);
```

# FBO

FBO：Framebuffer Object，帧缓冲区。默认情况下，OpenGL 将使用窗口系统提供的帧缓冲区（默认帧缓冲区）作为绘图表面，如果应用程序只在屏幕的表面上绘图，则使用默认帧缓冲区通常很高效，但如果需要渲染到纹理，或者希望渲染操作在后台进行，而不是显示到屏幕上，就需要创建自己的帧缓冲区了。

## 帧缓冲区对象

创建：

```
unsigned int fbo;
glGenFramebuffers(1, &fbo);
```

绑定：
`glBindFramebuffer(GL_FRAMEBUFFER, fbo);`
在绑定到 GL_FRAMEBUFFER 目标之后，当前绑定的 FBO 将成为所有针对帧缓冲区的读取和写入操作的目标。还可以使用 GL_READ_FRAMEBUFFER 或 GL_DRAW_FRAMEBUFFER，将 FBO 分别绑定到读取目标或写入目标。绑定到 GL_READ_FRAMEBUFFER 的 FBO 将成为类似 glReadPixels 的读取操作的目标，而绑定到 GL_DRAW_FRAMEBUFFER 的 FBO 会被用作渲染、清除等写入操作的目标。

恢复为默认帧缓冲区：
`glBindFramebuffer(GL_FRAMEBUFFER, 0);`
删除：
`glDeleteFramebuffers(1, &fbo);`
## 纹理附件

通常会把一个纹理附加到帧缓冲区，之后所有的渲染指令将会写入到这个纹理中。

生成一个纹理并附着到缓冲区中：

```
unsigned int texture;
glGenTextures(1, &texture);
glBindTexture(GL_TEXTURE_2D, texture);

glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
```

可以给纹理的 data 参数传递 NULL，为这个纹理分配内存而不填充像素数据。

除了颜色附件之外，还可以附加深度缓冲区、模板缓冲区纹理到帧缓冲区对象中，但这两个主要用于实现 3D 效果，音视频中用得比较少，Renderbuffer Object 同理，这里略过。

## glDrawBuffers

如果要将颜色输出到多个缓冲区，着色器必须配置为写入多重颜色输出：

```
layout(location = 0) out vec4 fragData0;
layout(location = 1) out vec4 fragData1;
layout(location = 2) out vec4 fragData2;
layout(location = 3) out vec4 fragData3;
```

默认情况下，单独的颜色输出将被发送到颜色输出 0（即 GL_COLOR_ATTACHMENT0），如果不告诉 OpenGL 如何处理着色器输出，那么只有第一个输出将被通过。可以使用 glDrawBuffers 来对着色器输出进行路由：

```
const GLenum attachments[4] = {
    GL_COLOR_ATTACHMENT0,
    GL_COLOR_ATTACHMENT1,
    GL_COLOR_ATTACHMENT2,
    GL_COLOR_ATTACHMENT3
};

glDrawBuffers(4, attachments);
```

attachments 数组的索引对应于着色器中的 location 值， 数组中的值只能为 GL_NONE、GL_BACK、GL_COLOR_ATTACHMENTn（n 的取值范围为 0 ~ GL_MAX_COLOR_ATTACHMENTS）

![glDrawArrays](https://img-blog.csdn.net/20180818111230145?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## glBlitFramebuffer

如果要使用帧缓冲区中的图像数据，传统的做法是将它们从 GPU 复制到 CPU，再从 CPU 复制到 GPU，这非常低效，glBlitFramebuffer（blit：bit-level-image-transfer）就是解决这个问题的，它能将像素数据从一个点移动到另一个点。如果将读取和绘制缓冲区设置为同一个 FBO，并将同一个 FBO绑定到了 GL_READ_FRAMEBUFFER、GL_DRAW_FRAMEBUFFER，那么甚至可以将数据从一个缓冲区的一部分复制到另一个缓冲区：

```
GLint width = 800;
GLint height = 600;
GLenum fboBuffs[] = { GL_COLOR_ATTACHMENT0 };
glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboName);
glBindFramebuffer(GL_READ_FRAMEBUFFER, fboName);
glDrawBuffers(1, fboBuffs);
glReadBuffer(GL_COLOR_ATTACHMENT0);
glBlitFramebuffer(0, 0, width, height,
                 (width *0.8), (height*0.8), width, height,
                 GL_COLOR_BUFFER_BIT, GL_LINEAR );
```



