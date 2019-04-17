# OpenGL ES 3.0（四）图元、VBO、VAO - 我的学习笔记 - CSDN博客





2018年08月11日 21:59:45[zouzhiheng](https://me.csdn.net/u011330638)阅读数：71








# 图元

OpenGL ES 3.0 可供绘制的图元类型有： 

    1) Triangles，三角形 

    2) Lines，线 

    3) Point sprites，点
## 三角形

绘制三角形时，可供选择的绘制方式有：GL_TRIANGLES、GL_TRIANGLE_STRIP、GL_TRIANGLE_FAN 三种，效果如下：

![三角形的三种绘制方式](https://img-blog.csdn.net/20180811215539623?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

即：

1) GL_TRIANGLES，共有 n/3 个三角形会被绘制，绘制的方式是：(v0, v1, v2)、(v3, v4, v5) ……

2) GL_TRIANGLE_STRIP，共有 n-2 个三角形会被绘制，绘制的方式是：(v0, v1, v2)、(v2, v1, v3)、(v2, v3, v4)……

3) GL_TRIANGLE_FAN，共有 n-2 个三角形会被绘制，绘制的方式是：(v0, v1, v2)、(v0, v2, v3)、(v0, v3, v4)……

## 线

支持   GL_LINES、GL_LINE_STRIP、GL_LINE_LOOP 三种，效果如下：

![线的三种绘制方式](https://img-blog.csdn.net/20180811215639883?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

可以使用 glLineWidth 指定线的宽度。

## 点

多用于游戏，未研究，略

# 图元绘制

除了使用 glDrawArrays 绘制之外，还可以使用 glDrawElements。glDrawArrays 的缺点是顶点不共享，比如：

![正方体](https://img-blog.csdn.net/20180811215717100?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如果要用 glDrawArrays 绘制这样一个立方体，则由于顶点不共享，因此每个面都需要 4 个顶点存储数据，一共有六个面，需要 24 个顶点数据。

而使用 glDrawElements，则可以定义顶点索引，指定每次绘制一个三角形时，所用到的顶点，比如绘制一个正方形：

```cpp
const static GLfloat VERTICES[] = {
        -0.5f, 0.5f, 0.0f,  // 顶点 0
        -0.5f, -0.5f, 0.0f, // 顶点 1
        0.5f, -0.5f, 0.0f,  // 顶点 2
        0.5f, 0.5f, 0.0f,   // 顶点 3
};

const static GLfloat COLORS[]{
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 1.0f
};

const static GLushort INDICES[] = {
        0, 1, 2, // 使用顶点 0、1、2 绘制第一个三角形
        0, 2, 3  // 使用顶点 0、2、3 绘制第二个三角形
};
```

值得注意的是，图元绘制的方式有两种，上述顶点数据和颜色数据分开的属于第一种，还可以把顶点和颜色数据放在同一个数据里：

```cpp
const static GLfloat VERTICES[] = {
        -0.5f, 0.5f, 0.0f,      // 顶点 0
        0.0f, 1.0f, 0.0f, 1.0f,   // 颜色 0
        -0.5f, -0.5f, 0.0f,     // 顶点 1
        1.0f, 0.0f, 0.0f, 1.0f,   // 颜色 1
        0.5f, -0.5f, 0.0f,      // 顶点 2
        0.0f, 0.0f, 1.0f, 1.0f,   // 颜色 2
        0.5f, 0.5f, 0.0f,       // 顶点 3
        1.0f, 0.0f, 1.0f, 1.0f    // 颜色 3
};
```

只需要在设置顶点、颜色数据的时候指定 stride 和 offset 即可：

```
const static GLint VERTEX_POS_SIZE = 3;
const static GLint VERTEX_COLOR_SIZE = 4;
const static GLsizei VERTEX_STRIDE = sizeof(GLfloat) * (VERTEX_POS_SIZE + VERTEX_COLOR_SIZE);

glVertexAttribPointer(ATTRIB_POSITION, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE,
                      VERTEX_STRIDE, VERTICES);
glVertexAttribPointer(ATTRIB_COLOR, VERTEX_COLOR_SIZE, GL_FLOAT, GL_FALSE,
                      VERTEX_STRIDE, VERTICES[3]); // 3 为第一个颜色数据在数组中的位置
```

# Vertex Buffer Object

存储在应用程序中的顶点数据，在每次调用 glDrawArrays、glDrawElements 的时候，必须从 CPU 内存复制到 graphics 内存中，复制的操作显得十分繁琐且降低了程序的运行效率。Vertex Buffer Object(VBO) 可以很好地解决这个问题，它能在绘制图元之前缓冲顶点或索引数据，避免每次绘制图元时都要重新发送数据。

使用示例（继续上面的例子）：

1) 生成 VBO 缓存数据

```
// 生成 3 个 VBO
glGenBuffers(3, mVboIds);
// GL_ARRAY_BUFFER、GL_ELEMENT_ARRAY_BUFFER 是可选的两个 buffer 目标
glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
// 缓存顶点数据
glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);

// 缓存颜色数据
glBindBuffer(GL_ARRAY_BUFFER, mVboIds[1]);
glBufferData(GL_ARRAY_BUFFER, sizeof(COLORS), COLORS, GL_STATIC_DRAW);

// 缓存索引数据
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[2]);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);
```

2) 使用 VBO 绘制图元

```
glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
glEnableVertexAttribArray(ATTRIB_POSITION);
glVertexAttribPointer(ATTRIB_POSITION, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE,
                      sizeof(GLfloat) * VERTEX_POS_SIZE, 0);

glBindBuffer(GL_ARRAY_BUFFER, mVboIds[1]);
glEnableVertexAttribArray(ATTRIB_COLOR);
glVertexAttribPointer(ATTRIB_COLOR, VERTEX_COLOR_SIZE, GL_FLOAT, GL_FALSE,
                      sizeof(GLfloat) * VERTEX_COLOR_SIZE, 0);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[2]);

glDrawElements(GL_TRIANGLES, INDEX_NUMBER, GL_UNSIGNED_SHORT, 0);

glDisableVertexAttribArray(ATTRIB_POSITION);
glDisableVertexAttribArray(ATTRIB_COLOR);

glBindBuffer(GL_ARRAY_BUFFER, 0);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
```

# Vertex Array Objects

VAO 是 OpenGL ES 3.0 引入的新特性，可以使图元的绘制更高效。VBO 用于缓存顶点、颜色、索引等数据，VAO 则可以认为是用于缓存 VBO 的容器：

```
glGenVertexArrays(1, &mVaoId);
glBindVertexArray(mVaoId);

glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
glEnableVertexAttribArray(ATTRIB_POSITION);
glVertexAttribPointer(ATTRIB_POSITION, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE,
                      sizeof(GLfloat) * VERTEX_POS_SIZE, 0);

glBindBuffer(GL_ARRAY_BUFFER, mVboIds[1]);
glEnableVertexAttribArray(ATTRIB_COLOR);
glVertexAttribPointer(ATTRIB_COLOR, VERTEX_COLOR_SIZE, GL_FLOAT, GL_FALSE,
                      sizeof(GLfloat) * VERTEX_COLOR_SIZE, 0);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[2]);

glBindVertexArray(0);
glBindBuffer(GL_ARRAY_BUFFER, 0);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
```

绘制图元时，代码就会变得非常简单：

```
glBindVertexArray(mVaoId);
glDrawElements(GL_TRIANGLES, INDEX_NUMBER, GL_UNSIGNED_SHORT, 0);
glBindVertexArray(0);
```

源码已上传到 [GitHub](https://github.com/zouzhiheng/AVGraphics)。




