# 梳理 Opengl ES 3.0 （二）剖析一个GLSL程序 - DesignYourDream - 博客园
# [梳理 Opengl ES 3.0 （二）剖析一个GLSL程序](https://www.cnblogs.com/designyourdream/p/6739413.html)
OpenGL ES shading language 3.0 也被称作 GLSL，是个 C风格的编程语言。
![](https://images2015.cnblogs.com/blog/698097/201704/698097-20170420162422852-2065825955.png)
Opengl ES 3.0内部有两种可编程处理单元，即Vertex processor和Fragment processor，分别用来处理Vertex shader executable和Fragment shader executable。注意，Opengl ES 3.0不支持Geometry Shader。上图中，紫色部分就是可执行体了，即 executable .
 先来一段Vertex shader代码
```
#version 300 es //版本号
in vec4 VertexPosition; //应用层输入逐顶点位置坐标数据
in vec4 VertexColor; //应用层输入逐顶点颜色数据
uniform float RadianAngle; //应用层输入数据
out vec4 TriangleColor;
mat2 rotation = mat2(cos(RadianAngle),sin(RadianAngle),
-sin(RadianAngle),cos(RadianAngle));
void main()
{
　　gl_Position = mat4(rotation)*VertexPosition;//给内置数据赋值
***　　TriangleColor = VertexColor;***
***}***
```
再来一段Fragment shader代码
```
#version 300 es
precision mediump float;//配置精度
in vec4 TriangleColor;
out vec4 FragColor;
void main() {
FragColor = TriangleColor;
};
```
 上面这两段代码，只是文本数据，上述的两种processor可没办法直接执行，这就需要一个叫做编译和链接的步骤，来将这个文本数据变成executable的数据。可以通过下图来了解这个创建executable的过程：
![](https://images2015.cnblogs.com/blog/698097/201704/698097-20170420163726899-1427456626.png)
需要在应用层加载和编译shader，使用如下代码进一步说明：
```
GLuint loadAndCompileShader(GLenum shaderType, const char* sourceCode) 
{
　　// Create the shader
　　GLuint shader = glCreateShader(shaderType);
　　if ( shader ) 
　　{
　　　　// Pass the shader source code
　　　　glShaderSource(shader, 1, &sourceCode, NULL);
　　　　// Compile the shader source code
　　　　glCompileShader(shader);
　　　　// Check the status of compilation
　　　　GLint compiled = 0;
　　　　glGetShaderiv(shader,GL_COMPILE_STATUS,&compiled);
　　　　if (!compiled)
　　　　{
　　　　// Get the info log for compilation failure
　　　　GLint infoLen = 0;
　　　　glGetShaderiv(shader,GL_INFO_LOG_LENGTH, &infoLen);
　　　　if (infoLen) 
　　　　{
　　　　　　char* buf = (char*) malloc(infoLen);
　　　　　　if (buf) 
　　　　　　{
　　　　　　　　glGetShaderInfoLog(shader, infoLen, NULL, buf);
　　　　　　　　printf("Could not compile shader %s:" buf);
　　　　　　　　free(buf);
　　　　　　}
　　　　// Delete the shader program
　　　　glDeleteShader(shader);
　　　　shader = 0;
　　　　}
　　}
}
return shader;
}
```
使用如下代码来执行链接过程：
```
GLuint linkShader(GLuint vertShaderID,GLuint fragShaderID){
if (!vertShaderID || !fragShaderID){ // Fails! return
return 0;
}
// Create an empty program object
GLuint program = glCreateProgram();
if (program) {
// Attach vertex and fragment shader to it
glAttachShader(program, vertShaderID);
glAttachShader(program, fragShaderID);
// Link the program
glLinkProgram(program);
GLint linkStatus = GL_FALSE;
glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
if (linkStatus != GL_TRUE) {
GLint bufLength = 0;
glGetProgramiv(program, GL_INFO_LOG_LENGTH,
&bufLength);
if (bufLength) {
char* buf = (char*) malloc(bufLength);
if(buf) { glGetProgramInfoLog(program,bufLength,NULL,buf);
printf("Could not link program:\n%s\n", buf);
free(buf);
}
}
glDeleteProgram(program);
program = 0;
}
}
return program;
}
```
前文一直提到的executable就是这个返回的句柄变量 program。这个program将会在流水线的Processor上执行。
回过头再来分析vertex shader代码和fragment shader代码。
```
in vec4 VertexPosition;
in vec4 VertexColor;
```
shader代码里声明的这两个变量的值是从哪里获取的呢，这就涉及了一个重要的环节，就是应用层和shader层的数据通信问题。为了方便理解，根据执行的位置不同，可以把应用层看做CPU，把shader层看做GPU。即可抽象为CPU与GPU通信，进一步抽象为客户端C和服务端S之间的通信。下面来解释这个通信的流程，从通信上来说，必然是要先让客户端找到服务器端的一个通信端口，然后客户端建立与这个通信端口的连接，最后只要往这个连接上扔数据，这样服务端就能收到数据了。
首先实现寻找到服务端通信端口的功能，以下代码就帮助CPU端找到GPU端的数据通信端口
```
GLuint positionAttribHandle;
GLuint colorAttribHandle;
positionAttribHandle = glGetAttribLocation(programID, "VertexPosition");
colorAttribHandle = glGetAttribLocation(programID, "VertexColor");
```
以下代码，实现了往通信通道上扔数据的功能：
```
// Send data to shader using queried attrib location
glVertexAttribPointer(positionAttribHandle, 2, GL_FLOAT,GL_FALSE, 0, gTriangleVertices);
glVertexAttribPointer(colorAttribHandle, 3, GL_FLOAT,GL_FALSE, 0, gTriangleColors);
```
GPU端，默认所有这些顶点属性端口都是关闭的，因此需要打开这些通信端口：
```
// Enable vertex position attribute
glEnableVertexAttribArray(positionAttribHandle);
glEnableVertexAttribArray(colorAttribHandle);
```
这样，一条通信信道就算建立起来了。

