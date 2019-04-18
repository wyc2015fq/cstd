# 【OpenGL4.0】GLSL-使用Uniform Block实现着色器的数据共享 - 江南烟雨 - CSDN博客
2012年07月03日 08:17:21[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：8420
**一、在GLSL中使用Uniform Block**
在GLSL渲染语言中，我们经常采用缓冲区来存储Uniform型的Block。
比如我们需要绘制下面这样的效果：
![](https://img-my.csdn.net/uploads/201207/02/1341235323_2963.jpg)
在这里我们绘制了一个圆，圆内部颜色和外部颜色不同，而且边缘部分，颜色是平滑过渡的。
我们这样来实现这一效果：
首先定义一个内径和外径。然后给这个正方形分配纹理坐标，然后通过纹理坐标离(0.5,0.5)的距离与内径和外径的关系来控制当前像素点的颜色。
我们要在Shader中提供这样的一些信息：圆的内径及外径、内部颜色和外部颜色。采用Block来将这些信息封装在一起是合适的选择。
采用Block的原因是：
如果你的程序中包含了多个着色器，而且这些着色器使用了相同的Uniform变量，你就不得不为每个着色器分别管理这些变量。Uniform变量的location是在程序链接的时候产生的，因此Uniform变量的location会随着着色器的不同而发生变化。因此，这些Uniform变量的数据必须重新产生，然后应用到新的location上。
而Uniform Block正是为了使在着色器间共享Uniform数据变得更加容易而设计的。有了Uniform Block，我们可以创建一个缓冲区对象来存储这些Uniform变量的值，然后将缓冲区对象绑定到Uniform Block。当着色器程序改变的时候，只需要将同样的缓冲区对重新绑定到在新的着色器中与之相关的Block即可。
Uniform Block的定义和C/C++中的struct很类似。在本文的例子中我们需要定义的Uniform Block为：
方法一：
```cpp
uniform BlobSettings {
  vec4 InnerColor;
  vec4 OuterColor;
  float RadiusInner;
  float RadiusOuter;
};
```
或者是
方法二：
```cpp
uniform BlobSettings{
	vec4 InnerColor;
	vec4 OuterColor;
	float RadiusInner;
	float RadiusOuter;
}Blob;
```
这样我们就定义了一个名为BlobSettings的Block，包含了4个uniform 变量。
在方法一中，Block中的变量依然是全局域的一部分，取用的时候不用Block名。
而在方法二中，Block中的变量则是在Blob名字域中，取用的时候需要加上Block名。
这两种方法在使用上有少许差别。
用来存储uniform数据的缓冲区对象称作UBO（uniform buffer object）。它只是一个绑定到特定location的缓冲区对象。
注意下面的代码是基于上述定义Block的方法二的基础之上的。
下面贴出完整的GLSL渲染器代码：
basic_block.vert:
```cpp
#version 400
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexTexCoord;
out vec3 TexCoord;
void main()
{
	TexCoord = VertexTexCoord;
	gl_Position = vec4(VertexPosition,1.0);
}
```
basic_block.frag:
```cpp
#version 400
in vec3 TexCoord;
layout (location = 0) out vec4 FragColor;
uniform BlobSettings{
	vec4 InnerColor;
	vec4 OuterColor;
	float RadiusInner;
	float RadiusOuter;
}Blob;//Use an instance name with a uniform block
void main()
{
	float dx = TexCoord.x - 0.5;
	float dy = TexCoord.y - 0.5;
	float dist = sqrt(dx * dx + dy * dy);
	//smoothstep : perform Hermite interpolation between two values
	FragColor = mix(Blob.InnerColor,Blob.OuterColor,
			smoothstep(Blob.RadiusInner,Blob.RadiusOuter,dist));
}
```
UBO的设置：
```cpp
void Scenebasic_block::setUniformBlock()
{
	GLuint blockIndex = glGetUniformBlockIndex(prog.getHandle(),
												"BlobSettings");
	GLint blockSize;
	glGetActiveUniformBlockiv(prog.getHandle(),
							blockIndex,
							GL_UNIFORM_BLOCK_DATA_SIZE,
							&blockSize);
	GLubyte *blockBuffer = (GLubyte*)malloc(blockSize);
	//Query for the offsets of the each block variable
	//layout of the data within a uniform block is implementation dependent
	const GLchar *names[] = {"BlobSettings.InnerColor","BlobSettings.OuterColor",
							"BlobSettings.RadiusInner","BlobSettings.RadiusOuter"};
	GLuint indices[4];
	glGetUniformIndices(prog.getHandle(), 4, names, indices);
	GLint offset[4];
	glGetActiveUniformsiv(prog.getHandle(), 4, indices, GL_UNIFORM_OFFSET, offset);
	GLfloat outerColor[] = {0.0f, 0.0f, 0.0f, 0.0f};
	GLfloat innerColor[] = {1.0f, 1.0f, 0.75f, 1.0f};
	GLfloat innerRadius = 0.25f, outerRadius = 0.45f;
	memcpy(blockBuffer + offset[0],innerColor,4 * sizeof(float));
	memcpy(blockBuffer + offset[1],outerColor,4 * sizeof(float));
	memcpy(blockBuffer + offset[2],&innerRadius,sizeof(float));
	memcpy(blockBuffer + offset[3],&outerRadius,sizeof(float));
	//create OpenGL buffer UBO to store the data
	GLuint uboHandle;
	glGenBuffers(1,&uboHandle);
	glBindBuffer(GL_UNIFORM_BUFFER,uboHandle);
	glBufferData(GL_UNIFORM_BUFFER,blockSize,blockBuffer,
					GL_DYNAMIC_DRAW);
	//bind the UBO th the uniform block
	glBindBufferBase(GL_UNIFORM_BUFFER,blockIndex,uboHandle);
	free(blockBuffer);
}
```
**二、在Uniform Block中使用布局标识符std140**
**1、std140的使用**
由于UBO中数据的布局和具体实现有关（内存对齐等等），因此我们需要在设置其值的时候查询偏移量。然后，我们可以使用标准布局std40来避免这一操作。这可以通过在定义Uniform Block的时候使用布局标识符来实现：
layout( std140 ) uniform BlobSettings {
   …
};
**2、std140详解**
std40布局标识符要求GLSL着色器依据一组规则来组织Uniform Block中的变量，这样使得我们能够预先计算出其中的变量的偏移量。
变量的偏移量是根据起始偏移量和在其之前的变量的大小累积计算得到的。第一个成员变量的起始偏移量总是0.
**std40的布局规则**
|**变量类型**|**变量大小/偏移量**|
|----|----|
|标量数据类型（bool,int,uint,float）|基于基本机器类型的标量值大小（例如，sizeof(GLfloat)）|
|二元向量(bvec2,ivec2,uvec2,vec2)|标量类型大小的两倍|
|三元向量(bvec3,ivec3,uvec3,vec3)|标量类型大小的四倍|
|三元向量(bvec4,ivec4,uvec4,vec4)|标量类型大小的四倍|
|标量的数组或向量|数组中每个元素大小是基本类型的大小，偏移量是其索引值（从0开始）与元素大小的乘积。整个数组必须是vec4类型的大小的整数倍（不足将在尾部填充）。|
|一个或多个C列R行列主序矩阵组成的数组|以C个向量（每个有R个元素）组成的数组形式存储。会像其他数组一样填充。如果变量是M个列主序矩阵的数组，那么它的存储形式是：M*C个向量（每个有R个元素）组成的数组。|
|一个或多个R行C列的行主序矩阵组成的数组|以R个向量（每个有C个元素）组成的数组。默认像其他数组一样填充。如果变量是M个行主序矩阵组成的数组，则存储形式是M*R个向量（每个有C个元素）组成的数组。|
|单个结构体或多个结构体组成的数组|单个结构体成员的偏移量和大小可以由前面的规则计算出。结构大小总是vec4大小的整数倍（不足在后面补齐）。由结构组成的数组，偏移量的计算需要考虑单个结构的对齐和补齐。结构的成员偏移量由前面的规则计算出。|
