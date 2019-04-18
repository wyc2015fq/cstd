# 【OpenGL4.0】GLSL渲染语言入门与VBO、VAO使用：绘制一个三角形 - 江南烟雨 - CSDN博客
2012年06月03日 16:15:43[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：42072
以前都是用Cg的，现在改用GLSL，又要重新学，不过两种语言很多都是相通的。
下面的例子是实现绘制一个三角形的简单程序。采用了VBO（veretx buffer object）、VAO（vertex array object）等OpenGL的一些新特性。越往后发展，可编程管线肯定是大势所趋，OpenGL里原来的一些固定管线的内容肯定会被废弃掉。所以从现在开始写程序就要养成使用新特性、采用可编程管线技术的好习惯。
**一、VAO、VBO介绍**
随着OpenGL状态和固定管线模式的移除，我们不在用任何glEnable函数调用，而且也不会有glVertex、glColor等函数调用。这就意味着我们需要一种新的方式来将数据传输到图形卡以渲染图形。我们可以采用VBO，或者是在OpenGL3以上版本引入的新的特性，叫做VAO。通过它，我们可以把顶点数据和颜色存储在不同的VBO中，但是在同一个VAO中。对于法线数据或者其他的顶点信息也是一样。
VAO，是这样一种方式：把对象信息直接存储在图形卡中，而不是在当我们需要的时候传输到图形卡。这就是Direct3D所采用得方式，而在OpenGL中只有OpenGL3.X以上的版本中采用。这就意味着我们的应用程序不用将数据传输到图形卡或者是从图形卡输出，这样也就获得了额外的性能提升。
使用VAO并不难。我们不需要大量的glVertex调用，而是把顶点数据存储在数组中，然后放进VBO，最后在VAO中存储相关的状态。记住：VAO中并没有存储顶点的相关属性数据。OpenGL会在后台为我们完成其他的功能。
**使用VAO的步骤：**
**1、产生VAO**
void glGenVertexArrays(GLsizei n,
  GLuint *arrays);
n：要产生的VAO对象的数量。
arrays：存放产生的VAO对象的名称。
**2、绑定VAO**
void glBindVertexArray(GLuint array);
array：要绑定的顶点数组的名字。
**3、产生VBOs**
void glGenBuffers(GLsizei   n,
  GLuint *   buffers);
产生缓冲区对象的名称。
参数含义和glGenVertexArrays类似。
**4、绑定VBOs**
void glBindBuffer(GLenum   target,
  GLuint   buffer);
绑定一个缓冲区对象。
target可能取值是：GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_PIXEL_PACK_BUFFER, or GL_PIXEL_UNPACK_BUFFER.
当进行绑定之后，以前的绑定就失效了。
**5、给VBO分配数据: **
void glBufferData(   GLenum          target,
        GLsizeiptr      size,
        const GLvoid *        data,
        GLenum          usage);
target可能取值为：GL_ARRAY_BUFFER（表示顶点数据）, GL_ELEMENT_ARRAY_BUFFER（表示索引数据）,GL_PIXEL_PACK_BUFFER（表示从OpenGL获取的的像素数据）, or GL_PIXEL_UNPACK_BUFFER（表示传递给OpenGL的像素数据）.
参数含义：
size：缓冲区对象字节数
data：指针：指向用于拷贝到缓冲区对象的数据。或者是NULL，表示暂时不分配数据。
**6、定义存放顶点属性数据的数组：**
首先需要启用VAO中对应的顶点属性数组：
void glEnableVertexAttribArray(      GLuint    index);
index：指定了需要启用的顶点属性数组的索引
注意：它只在OpenGL2.0及其以上版本才有。
**7、给对应的顶点属性数组指定数据：**
void glVertexAttribPointer(      GLuint    index,
        GLint      size,
        GLenum          type,
        GLboolean     normalized,
        GLsizei  stride,
        const GLvoid *        pointer);
index：要指定数据的顶点属性数组的索引。
size：每个顶点属性的数据个数。可能的取值是1、2、3或者4.初始值是4.
type：数组中每个数据的类型。可能的取值是：GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_FLOAT, or GL_DOUBLE。初始值是GL_FLOAT。
normalized：指定顶点数在被访问的时候是否需要被归一化。
注意：如果有个非零的缓冲对象绑定到GL_ARRAY_BUFFER，那么pointer就是对应的缓冲区对象的偏移量。
 stride：两个连续顶点的属性之间的偏移量。
pointer：指向数组中的第一个顶点属性的第一个数据。
**8、然后在进行渲染的时候，只需要绑定对应的VAO即可，操作起来十分方便。**
glBindVertexArray(vaoHandle);
**9、使用完毕之后需要清除绑定。**
glBindVertexArray(0);
VAO参考资料：
[VAO wiki](http://www.opengl.org/wiki/Vertex_Array_Object)
[关于VAO的一篇博客](http://www.cppblog.com/init/archive/2012/02/21/166098.html)
[VAO](http://www.swiftless.com/tutorials/opengl4/4-opengl-4-vao.html)
事实上，在这个简单的程序中，不用VAO，只用VBO也一样可以实现。只是采用VAO可以进一步提升性能，而且在较新版本的OpenGL中不用VAO的方式会被逐渐废弃。
**二、GLSL入门**
关于GLSL，可以参考红宝书的附录，上面介绍了GLSL的入门知识。
这里也有一个很好的入门教程，英文的：
[GLSL入门tutorial](http://www.swiftless.com/tutorials/glsl/1_setup.html)
**三、程序实例**
下面贴出源代码（OpenGL，GLSL实现）：
basic.vert：
```cpp
//强制要求支持一个特定版本的GLSL版本
#version 400
in vec3 VertexPosition;
in vec3 VertexColor;
out vec3 Color;
void main()
{
	Color =VertexColor;
	gl_Position = vec4(VertexPosition,1.0);
}
```
basic.frag:
```cpp
#version 400
in vec3 Color;
out vec4 FragColor;
void main()
{
	FragColor = vec4(Color,1.0);
}
```
主文件：
main.cpp：
```cpp
#pragma comment(lib,"glew32.lib")
#include <GL/glew.h>
#include "textfile.h"
#include <GL/glut.h>
#include <iostream>
using namespace std;
GLuint vShader,fShader;//顶点着色器对象
//顶点位置数组
float positionData[] = {
	-0.8f, -0.8f, 0.0f,
	0.8f, -0.8f, 0.0f,
	0.0f,  0.8f, 0.0f };
//颜色数组
float colorData[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f };
GLuint vaoHandle;//vertex array object
void initShader(const char *VShaderFile,const char *FShaderFile)
{
	//1、查看GLSL和OpenGL的版本
	const GLubyte *renderer = glGetString( GL_RENDERER );
	const GLubyte *vendor = glGetString( GL_VENDOR );
	const GLubyte *version = glGetString( GL_VERSION );
	const GLubyte *glslVersion = 
		glGetString( GL_SHADING_LANGUAGE_VERSION );
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	cout << "GL Vendor    :" << vendor << endl;
	cout << "GL Renderer  : " << renderer << endl;
	cout << "GL Version (string)  : " << version << endl;
	cout << "GL Version (integer) : " << major << "." << minor << endl;
	cout << "GLSL Version : " << glslVersion << endl; 
	//2、编译着色器
	//创建着色器对象：顶点着色器
	vShader = glCreateShader(GL_VERTEX_SHADER);
	//错误检测
	if (0 == vShader)
	{
		cerr << "ERROR : Create vertex shader failed" << endl;
		exit(1);
	}
	//把着色器源代码和着色器对象相关联
	const GLchar *vShaderCode = textFileRead(VShaderFile);
	const GLchar *vCodeArray[1] = {vShaderCode};
	glShaderSource(vShader,1,vCodeArray,NULL);
	//编译着色器对象
	glCompileShader(vShader);
	//检查编译是否成功
	GLint compileResult;
	glGetShaderiv(vShader,GL_COMPILE_STATUS,&compileResult);
	if (GL_FALSE == compileResult)
	{
		GLint logLen;
		//得到编译日志长度
		glGetShaderiv(vShader,GL_INFO_LOG_LENGTH,&logLen);
		if (logLen > 0)
		{
			char *log = (char *)malloc(logLen);
			GLsizei written;
			//得到日志信息并输出
			glGetShaderInfoLog(vShader,logLen,&written,log);
			cerr << "vertex shader compile log : " << endl;
			cerr << log << endl;
			free(log);//释放空间
		}
	}
	//创建着色器对象：片断着色器
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	//错误检测
	if (0 == fShader)
	{
		cerr << "ERROR : Create fragment shader failed" << endl;
		exit(1);
	}
	//把着色器源代码和着色器对象相关联
	const GLchar *fShaderCode = textFileRead(FShaderFile);
	const GLchar *fCodeArray[1] = {fShaderCode};
	glShaderSource(fShader,1,fCodeArray,NULL);
	//编译着色器对象
	glCompileShader(fShader);
	//检查编译是否成功
	glGetShaderiv(fShader,GL_COMPILE_STATUS,&compileResult);
	if (GL_FALSE == compileResult)
	{
		GLint logLen;
		//得到编译日志长度
		glGetShaderiv(fShader,GL_INFO_LOG_LENGTH,&logLen);
		if (logLen > 0)
		{
			char *log = (char *)malloc(logLen);
			GLsizei written;
			//得到日志信息并输出
			glGetShaderInfoLog(fShader,logLen,&written,log);
			cerr << "fragment shader compile log : " << endl;
			cerr << log << endl;
			free(log);//释放空间
		}
	}
	//3、链接着色器对象
	//创建着色器程序
	GLuint programHandle = glCreateProgram();
	if (!programHandle)
	{
		cerr << "ERROR : create program failed" << endl;
		exit(1);
	}
	//将着色器程序链接到所创建的程序中
	glAttachShader(programHandle,vShader);
	glAttachShader(programHandle,fShader);
	//将这些对象链接成一个可执行程序
	glLinkProgram(programHandle);
	//查询链接的结果
	GLint linkStatus;
	glGetProgramiv(programHandle,GL_LINK_STATUS,&linkStatus);
	if (GL_FALSE == linkStatus)
	{
		cerr << "ERROR : link shader program failed" << endl;
		GLint logLen;
		glGetProgramiv(programHandle,GL_INFO_LOG_LENGTH,
			&logLen);
		if (logLen > 0)
		{
			char *log = (char *)malloc(logLen);
			GLsizei written;
			glGetProgramInfoLog(programHandle,logLen,
				&written,log);
			cerr << "Program log : " << endl;
			cerr << log << endl;
		}
	}
	else//链接成功，在OpenGL管线中使用渲染程序
	{
		glUseProgram(programHandle);
	}
}
void initVBO()
{
	// Create and populate the buffer objects
	GLuint vboHandles[2];
	glGenBuffers(2, vboHandles);
	GLuint positionBufferHandle = vboHandles[0];
	GLuint colorBufferHandle = vboHandles[1];
	//绑定VBO以供使用
	glBindBuffer(GL_ARRAY_BUFFER,positionBufferHandle);
	//加载数据到VBO
	glBufferData(GL_ARRAY_BUFFER,9 * sizeof(float),
		positionData,GL_STATIC_DRAW);
	//绑定VBO以供使用
	glBindBuffer(GL_ARRAY_BUFFER,colorBufferHandle);
	//加载数据到VBO
	glBufferData(GL_ARRAY_BUFFER,9 * sizeof(float),
		colorData,GL_STATIC_DRAW);
	glGenVertexArrays(1,&vaoHandle);
	glBindVertexArray(vaoHandle);
	glEnableVertexAttribArray(0);//顶点坐标
	glEnableVertexAttribArray(1);//顶点颜色
	//调用glVertexAttribPointer之前需要进行绑定操作
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );
}
void init()
{
	//初始化glew扩展库
	GLenum err = glewInit();
	if( GLEW_OK != err )
	{
		cout <<"Error initializing GLEW: " << glewGetErrorString(err) << endl;
	}
	initShader("basic.vert","basic.frag");
	initVBO();
	glClearColor(0.0,0.0,0.0,0.0);
	//glShadeModel(GL_SMOOTH);
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	////绘制一个三角形(使用普通方法)
	//glBegin(GL_TRIANGLES);
	//glColor3f(0.0f,1.0f,0.0f);
	//glVertex3f(0.0f,1.0f,0.0f);
	//glColor3f(0.0f,1.0f,0.0f);
	//glVertex3f(-1.0f,-1.0f,0.0f);
	//glColor3f(0.0f,0.0f,1.0f);
	//glVertex3f(1.0f,-1.0f,0.0f);
	//glEnd();
	//使用VAO、VBO绘制
	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLES,0,3);
	glBindVertexArray(0);
	glutSwapBuffers();
}
void keyboard(unsigned char key,int x,int y)
{
	switch(key)
	{
	case 27:
		glDeleteShader(vShader);
		glUseProgram(0);
		break;
	}
}
int main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600,600);
	glutInitWindowPosition(100,100);
	glutCreateWindow("GLSL Test : Draw a triangle");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	
	glutMainLoop();
	return 0;
}
```
读取shader文件的程序：
textfile.h：
```cpp
// textfile.h: interface for reading and writing text files
#ifndef TEXTFILE_H
#define TEXTFILE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *textFileRead(const char *fn);
int textFileWrite(char *fn, char *s);
unsigned char *readDataFromFile(char *fn);
#endif
```
textfile.cpp：
```cpp
// textfile.cpp
// simple reading and writing for text files
#include "textfile.h"
unsigned char * readDataFromFile(char *fn)
{
    FILE *fp;
    unsigned char *content = NULL;
    int count=0;
    if (fn != NULL) {
        fp = fopen(fn,"rb");
        if (fp != NULL) {
            fseek(fp, 0, SEEK_END);
            count = ftell(fp);
            rewind(fp);
            if (count > 0) {
                content = (unsigned char *)malloc(sizeof(unsigned char) * (count+1));
                count = fread(content,sizeof(unsigned char),count,fp);
                content[count] = '\0';
            }
            fclose(fp);
        }
    }
    return content;
}
char *textFileRead(const char *fn) {
    FILE *fp;
    char *content = NULL;
    int count=0;
    if (fn != NULL) {
        fp = fopen(fn,"rt");
        if (fp != NULL) {
            fseek(fp, 0, SEEK_END);
            count = ftell(fp);
            rewind(fp);
            if (count > 0) {
                content = (char *)malloc(sizeof(char) * (count+1));
                count = fread(content,sizeof(char),count,fp);
                content[count] = '\0';
            }
            fclose(fp);
        }
    }
    return content;
}
int textFileWrite(char *fn, char *s) {
    FILE *fp;
    int status = 0;
    if (fn != NULL) {
        fp = fopen(fn,"w");
        if (fp != NULL) {
            if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s))
                status = 1;
            fclose(fp);
        }
    }
    return(status);
}
```
运行结果：
![](https://img-my.csdn.net/uploads/201206/03/1338711557_2999.jpg)
![](https://img-blog.csdn.net/20180813092554459?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hpYWp1bjA3MDYxMjI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
