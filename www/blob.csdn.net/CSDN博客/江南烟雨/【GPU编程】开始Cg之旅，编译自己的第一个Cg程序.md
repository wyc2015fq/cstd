# 【GPU编程】开始Cg之旅，编译自己的第一个Cg程序 - 江南烟雨 - CSDN博客
2011年11月04日 21:57:25[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：13990
Cg（C for Graphcis）语言，是NVIDIA 与Microsoft 合作研发，旨在为开发人员提供一套方便、跨平台（良好的兼容性），控制可编程图形硬件的高级语言。Cg 语言的语法结构与C 语言非常类似，使用Cg 编写的着色程序默认的文件后缀是*. Cg。
首先从NVIDIA官方网站上下载Cg编译器：[http://developer.nvidia.com/cg-toolkit](http://developer.nvidia.com/cg-toolkit)。下载后直接安装即可。安装成功后在下列目录里会出现cgc.exe文件：C:\Program Files\NVIDIA Corporation\Cg\bin（可能会随着安装目录的不同而有所不同）。
Cg 语言规范是公开和开放的，并且 NVIDIA 开放了Cg 编译器技术的源代码，使用无限制的、免费的许可证。
目前还没有一个主流的专门为编写着色程序而开发的IDE，很多人都是直接在文本中写好程序后，然后将文件后缀改为.cg。在网上有一个名为NShader的Visual Studio2008 插件：[http://nshader.codeplex.com/](http://nshader.codeplex.com/)。安装之后可以支持编写着色程序。如图所示效果：
![](https://img-my.csdn.net/uploads/201204/18/1334750281_2610.jpg)
Cg有很多优点。Cg 同时被OpenGL 与Direct3D 两种编程API 所支持。这一点不但对开发人员而言非常方便，而且也赋予了Cg 程序良好的跨平台性。一个正确编写的Cg应用程序可以不做任何修改的同时工作在OpenGL 和Direct3D 之上。
Cg 语言通常采用动态编译的方式，即，在宿主程序运行时利用Cg 运行库( Cg46Runtime library)动态编译Cg 代码，使用动态编译的方式，可以将Cg 程序当作一个脚本，随时修改随时运行，节省大量的时间，在OGRE 图形引擎中就是采用这样的方法。在文献[2]的1.4.2 章节中提到Cg 语言同样支持静态编译方式，即，Cg 源码编译成汇编代码后，这部分目标代码被链接到宿主程序最后的可执行程序中。使用静态编译的好处是只要发布可执行文件即可，源码不会被公开。Cg
 编译器首先将Cg 程序翻译成可被图形API（OpenGL 和Direct3D）所接受的形式，然后应用程序使用适当的OpenGL 和Direct3D 命令将翻译后的Cg 程序传递给图形处理器，OpenGL 和Direct3D 驱动程序最后把它翻译成图形处理器所需要的硬件可执行格式。NVIDIA 提供的Cg 编译器为cgc.exe。Cg 程序的编译不但依赖于宿主程序所使用的三维编程接口，而且依赖于图形硬件环境，因为图形硬件自身的限制，不一定支持某种Cg 语句，例如，如果你所使用的GPU 并不支持循环控制指令，那么在Cg
 程序中编写的循环控制语句将无法通过编译。被特定的图形硬件环境或AIP 所支持的Cg 语言子集，被称为Cg Profiles。需要注意的是： profile 分为顶点程序的profile 和片段程序的profile，这是因为顶点着色器和片段着色器原本就不是工作在同一个硬件。Cg Profiles 是Cg 语言的重要组成部分，在使用Cg 语言编写着色程序时，首先要考虑的一点就是“当前的图形硬件环境支持那个Cg Profile”，这直接关系到您所编写的着色程序是否可以在当前的图形硬件上运行。
CGC的编译命令
如果Cg Toolkit 安装正确，在NVIDIA Corporation\Cg\bin 文件夹下会看到cgc.exe 文件。首先打开命令行窗口，输入“cgc –h”（引号不用输入），如果安装正确，则会出现图示的提示信息。
Cg 程序编译的命令形式为：
cgc [options] file
[options]表示可选配置项，file 表示Cg 程序文件名。可选配置项包括编译时选择使用的profile、着色程序的入口函数名称，以及着色程序文件名。比48
较典型的编译方式是：
cgc –profile glslv –entry main_v test.cg
-profile 是profile 配置项名；glslv 是当前所使用的profile 名称；-entry着色程序的入口函数名称配置项；main_v 是顶点着色程序的入口函数名；test.cg 是当前的着色程序文件名。编译器指定的着色程序入口函数名默认为main，通常为了将顶点\片段着色程序入口函数名区别开来，而并不使用默认名称。在下面所有的例子中，main_v 表示顶点着色程序入口函数名,main_f 表示片段着色程序入口函数名。
实例：编写vertex.cg文件：
```cpp
struct output_v
{
	float4 position:POSITION;
	float3 color:COLOR;
	float2 texCoord:TEXCOORD0;
};
output_v main_v(float2 position:POSITION,
						float3 color:COLOR,
						float2 texCoord:TEXCOORD0)
{
	output_v OUT;
	OUT.position = float4(position,0,1);
	OUT.color = color;
	OUT.texCoord = texCoord;
	
	return OUT;
}
```
将其放入C盘根目录下。
输入命令：cd C:/    和cgc -profile glslv -entry main_v test.cg
![](https://img-my.csdn.net/uploads/201204/18/1334750529_5227.jpg)
如果所写的程序中有错误，是可以检测出来的。
**C+++OpenGL+Cg程序实战1**：
绘制一盒六面体。
新建一个空工程，向其中添加一个.cpp文件和.cg文件。
.cpp文件代码：
```cpp
/* Minimalist program for using shaders, with no error checking */
#ifdef _MSC_VER
#pragma comment( lib, "cg.lib" )
#pragma comment( lib, "cgGL.lib" )
#endif
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
#endif
#include <Cg/cg.h>
#include <Cg/cgGL.h>
/*** Static Data ***/
/* New Cg global variables */
static CGcontext Context = NULL;
static CGprogram VertexProgram = NULL;
static CGparameter KdParam = NULL;
static CGparameter ModelViewProjParam = NULL;
static CGparameter VertexColorParam = NULL;
#ifdef __APPLE__
static CGprofile VertexProfile = CG_PROFILE_ARBVP1;
#else
static CGprofile VertexProfile = CG_PROFILE_VP20;
#endif
/* End new Cg global variables */
GLfloat CubeNormals[6][3] =//法向量
{
	{-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
	{0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0}
};
GLint CubeFaces[6][4] =//六个面的顶点的索引
{
	{0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
	{4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3}
};
GLfloat CubeVertices[8][3];//顶点坐标
 
static void DrawCube(void)//绘制立方体函数
{
	int i;
	cgGLBindProgram(VertexProgram);//bind a program to the current state
	/* Set various uniform parameters including the ModelViewProjection
	* matrix for transforming the incoming position into HPOS.*/
	if(KdParam != NULL)
		cgGLSetParameter4f(KdParam, 1.0, 1.0, 0.0, 1.0);
	/* Set the concatenate modelview and projection matrices */
	if(ModelViewProjParam != NULL)
		cgGLSetStateMatrixParameter(ModelViewProjParam,
			CG_GL_MODELVIEW_PROJECTION_MATRIX,
			CG_GL_MATRIX_IDENTITY);
	cgGLEnableProfile(VertexProfile);
/*
* Create cube with per-vertex varying attributes
*/
	for(i = 0; i < 6; i++)
	{
		glBegin(GL_QUADS);
		{
			glNormal3fv(&CubeNormals[i][0]);//设置当前法向量
			cgGLSetParameter3f(VertexColorParam, 1.0, 0.0, 0.0);
			glVertex3fv(&CubeVertices[CubeFaces[i][0]][0]);
			cgGLSetParameter3f(VertexColorParam, 0.0, 1.0, 0.0);
			glVertex3fv(&CubeVertices[CubeFaces[i][1]][0]);
			cgGLSetParameter3f(VertexColorParam, 0.0, 0.0, 1.0);
			glVertex3fv(&CubeVertices[CubeFaces[i][2]][0]);
			cgGLSetParameter3f(VertexColorParam, 1.0, 1.0, 1.0);
			glVertex3fv(&CubeVertices[CubeFaces[i][3]][0]);
		}
		glEnd();
	}
	cgGLDisableProfile(VertexProfile);
}
static void Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawCube();
	glutSwapBuffers();
}
static void InitializeCube(GLfloat v[8][3])//顶点坐标初始化
{
	/* Setup cube vertex data. */
	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;
}
static void InitializeGlut(int *argc, char *argv[])
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(Display);
	InitializeCube(CubeVertices);
	/* Use depth buffering for hidden surface elimination. */
	glEnable(GL_DEPTH_TEST);
	/* Setup the view of the cube. */
	glMatrixMode(GL_PROJECTION);
	gluPerspective( /* field of view in degree */ 40.0,
							/* aspect ratio */ 1.0,
							/* Z near */ 1.0, /* Z far */ 10.0);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0.0, 0.0, 5.0, /* eye is at (0,0,5) */
					0.0, 0.0, 0.0, /* center is at (0,0,0) */
					0.0, 1.0, 0.); /* up is in positive Y direction */
	/* Adjust cube position to be asthetic angle. */
	glTranslatef(0.0, 0.0, -1.0);
	#if 1
	glRotatef(60, 1.0, 0.0, 0.0);
	glRotatef(-20, 0.0, 0.0, 1.0);
	#endif
}
int main(int argc, char *argv[])
{
	InitializeGlut(&argc, argv);
	Context = cgCreateContext();/* Create one context which all shaders will use */
	VertexProgram = cgCreateProgramFromFile(Context,
		CG_SOURCE,
		"vertexShader.cg",
		VertexProfile,
		NULL, 
		NULL);/* Adds shader to the context */
	if(VertexProgram != NULL)
	{
		cgGLLoadProgram(VertexProgram);
		KdParam = cgGetNamedParameter(VertexProgram, "Kd");
		ModelViewProjParam = cgGetNamedParameter(VertexProgram, "ModelViewProj");
		VertexColorParam = cgGetNamedParameter(VertexProgram, "IN.VertexColor");
	}
	glutMainLoop();
	cgDestroyProgram(VertexProgram);
	cgDestroyContext(Context);
	return 0;
}
```
关于
```cpp
#ifndef GLUT_DISABLE_ATEXIT_HACK
#define GLUT_DISABLE_ATEXIT_HACK
#endif
```
防止对glut.lib 的连环依赖
.cg文件代码：
```
struct appdata
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float3 color : DIFFUSE;
	float3 VertexColor : SPECULAR;
};
struct vfconn
{
	float4 HPOS : POSITION;
	float4 COL0 : COLOR0;
};
vfconn main(appdata IN,
	uniform float4 Kd,
	uniform float4x4 ModelViewProj)
	{
	vfconn OUT;
	OUT.HPOS = mul(ModelViewProj, IN.position);
	OUT.COL0.xyz = Kd.xyz * IN.VertexColor.xyz;
	OUT.COL0.w = 1.0;
	return OUT;
} // main
```
运行效果：
![](http://hi.csdn.net/attachment/201112/30/0_13252348282ikf.gif)
代码解释：
.cpp文件：
DrawCube函数：绘制六面体：
cgGLBindProgram(VertexProgram);把一个程序和当前状态绑定。注意：在任意时刻只能有一个顶点程序和一个片段程序能够被绑定。所以绑定一个程序必须要把那个类型的其他程序解除绑定。
cgGLSetParameter4f(KdParam, 1.0, 1.0, 0.0, 1.0);：给参数赋值。
cgGLSetStateMatrixParameter( CGparameter param,
                                  CGGLenum matrix,
                                  CGGLenum transform );把当前Cg程序的模型视图变换矩阵告诉当前程序。给矩阵参数设置值。param:将要被设置值的参数矩阵。matrix:应该从当前状态中恢复的矩阵。transform:在矩阵被设置之前可能做的变换。
cgGLEnableProfile：在执行OpenGL程序之前，都要使相关profile可用。
然后是绘制六面体。
cgGLDisableProfile：应该马上停止着色器的使用，否则进行其他绘制时可能导致意想不到的情况。
display函数：
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)清除颜色缓冲区以及深度缓冲区。
glutSwapBuffers();如果当前窗口是双缓冲，则交换缓冲。 promotes the contents of the back buffer of the layer in use of the current window to become the contents of the front buffer. The contents of
 the back buffer then become undefined. The update typically takes place during the vertical retrace of the monitor, rather than immediately after glutSwapBuffers is called.
InitializeCube函数：顶点坐标初始化。
InitializeGlut函数：
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);指定初始显示模式：双缓冲、RGBA颜色模式和有深度缓冲。
glMatrixMode(GL_PROJECTION);指定当前矩阵：投影矩阵。
void gluPerspective(GLdouble  fovy,GLdouble  aspect,GLdouble  zNear,GLdouble  zFar);设定视图投影矩阵。fovy:视角(yf方向)。aspect:x、y比例。zNear和zFar分别表示近剪切平面和元剪切平面距离观察者的距离。、
glMatrixMode(GL_MODELVIEW);指定当前矩阵：模型视图矩阵。
gluLookAt：参数分别指定眼睛位置、参考点位置和向上的方向向量。
glTranslatef：平移变换。glRotatef:旋转变换。
main函数：
Context = cgCreateContext();创建上下文。是多个Cg脚本的容器。
cgCreateProgramFromFile：创建程序。程序可以以下列两种方式创建：
CGprogram cgCreateProgram(CGcontext    context, 
                          CGenum       programType,
                          const char*  program,
                          CGprofile    profile,
                          const char*  entry,
                          const char** args);
或者
CGprogram cgCreateProgramFromFile(CGcontext    context, 
                                  CGenum       programType,
                                  const char*  program,
                                  CGprofile    profile,
                                  const char*  entry,
                                  const char** args);
context:上下文句柄。profile：程序编译所要用的profile版本。entry：入口函数,NULL时用main。args一个指针，指向以NULL结束的数组，数组中元素是以NULL结束的string类型元素。两个函数的唯一区别是program被解释的方式不一样。cgCreateProgramFromFile中program是一个string，他是一个包含源代码的文件的名字。而cgCreateProgram，program直接包含了源代码。 If
 the enumerant programType is equal to CG_SOURCE, the source code is Cg source code; if it is equal to CG_OBJECT, the source code is precompiled object code and does not require any further compilation.
if(VertexProgram != NULL)如果程序创建成功，则加载程序。只需加载一次。
接下来的几行是参数绑定。把数据变量地址发送给Cg程序。
glutMainLoop();进入glut时间处理循环。
cgDestroyProgram(VertexProgram);cgDestroyContext(Context);销毁程序以及上下文。
.cg代码：
首先声明结构体appdata，包含了坐标、法向量、漫反射和镜面反射信息。
vfconn：包含了坐标和颜色信息。
main:
顶点处理入口函数：输出参数是appdate,返回vfconn类型。
还有两个uniform类型参数。Kd:材料镜面颜色。ModelViewProj：模型视图变换矩阵。
OUT.HPOS = mul(ModelViewProj, IN.position);模型视图变换
OUT.COL0.xyz = Kd.xyz * IN.VertexColor.xyz;:镜面反射后材料颜色计算。
最后返回vfconn型的OUT变量。
**C++ + OpenGL + Cg编程实战2：**
顶点Cg程序vertex.cg:
```cpp
struct output_v
{
	float4 position:POSITION;
	float3 color:COLOR;
	float2 texCoord:TEXCOORD0;
};
output_v main_v(float2 position:POSITION,
						float3 color:COLOR,
						float2 texCoord:TEXCOORD0)
{
	output_v OUT;
	OUT.position = float4(position,0,1);
	OUT.color = color;
	OUT.texCoord = texCoord;
	
	return OUT;
}
```
片断Cg程序：
```cpp
struct output_f 
{
	float4 color:COLOR;
};
output_f main_f(float4 color:COLOR)
{
	output_f OUT;
	OUT.color = color;
	
	return OUT;
}
```
C++程序main.cpp：
```cpp
#pragma comment(lib,"cg.lib")
#pragma comment(lib,"cgGL.lib")
#include <windows.h>
#include <GL/glut.h>
#include <CG/cg.h>
#include <CG/cgGL.h>
#include <iostream>
using namespace std;
//some global variables
static CGcontext context;
static CGprofile myCgVertexProfile,myCgFragmentProfile;
static CGprogram myCgVertexProgram,myCgFragmentProgram;
static const char *myProgramName = "CgSample4_varying_parameter",
						*myVertexProgramFileName = "vertex.cg",
						*myVertexProgramName = "main_v",
						*myFragmentProgramFileName = "fragment.cg",
						*myFragmentProgramName = "main_f";
bool CgUsed = true;
static void checkForCgError(const char *situation)
{
	CGerror error;
	const char *string = cgGetLastErrorString(&error);
	if (error != CG_NO_ERROR)
	{
		cout << myProgramName << " "
			<<situation << " "
			<< string << endl;
		if( CG_COMPILER_ERROR == error)
			cout << cgGetLastListing(context) << endl;
		exit(1);
	}
}
void init()
{
	glClearColor(1.0,1.0,1.0,1.0);
	glShadeModel(GL_SMOOTH);
	//init Cg 
	context = cgCreateContext();
	myCgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
	checkForCgError("selecting vertex profile");
	myCgVertexProgram = cgCreateProgramFromFile(
		context,
		CG_SOURCE,
		myVertexProgramFileName,
		myCgVertexProfile,
		myVertexProgramName,
		NULL);
	checkForCgError("Creating vertex Cg program from file");
	cgGLLoadProgram(myCgVertexProgram);
	checkForCgError("loading vertex program");
	myCgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
	checkForCgError("selecting fragment profile");
	myCgFragmentProgram = cgCreateProgramFromFile(
		context,
		CG_SOURCE,
		myFragmentProgramFileName,
		myCgFragmentProfile,
		myFragmentProgramName,
		NULL);
	checkForCgError("creating fragment program from file");
	cgGLLoadProgram(myCgFragmentProgram);
	checkForCgError("loading fragment program");
}
void keyboard(unsigned char key,int x,int y)
{
	switch(key)
	{
	case 'c':
		CgUsed = !CgUsed;
		glutPostRedisplay();
		break;
	case 27:
		cgDestroyProgram(myCgVertexProgram);
		cgDestroyProgram(myCgFragmentProgram);
		cgDestroyContext(context);
		exit(0);
		break;
	}
}
//display FPS on the title of the window
void displayFPS(){
	static float framesPerSecond    = 0.0f;       // This will store our fps
	static float lastTime   = 0.0f;       // This will hold the time from the last frame
	float currentTime = GetTickCount() * 0.001f;    
	++framesPerSecond;
	if( currentTime - lastTime > 1.0f )
	{
		framesPerSecond/=currentTime - lastTime;
		char strFrameRate[256];
		lastTime = currentTime;
		sprintf(strFrameRate, "Current Frames Per Second: %f", framesPerSecond);
		glutSetWindowTitle( strFrameRate);
		framesPerSecond = 0;
	}
}
void display()
{
	displayFPS();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (CgUsed)//enable Cg 
	{
		cout << "Cg is been used !" << endl;
		cgGLBindProgram(myCgVertexProgram);
		checkForCgError("binding vertex program");
		cgGLEnableProfile(myCgVertexProfile);
		checkForCgError("enabling vertex profile");
		cgGLBindProgram(myCgFragmentProgram);
		checkForCgError("binding fragment program");
		cgGLEnableProfile(myCgFragmentProfile);
		checkForCgError("enabling fragment profile");
	}
	else
		cout << "Cg is not used !" << endl;
	glBegin(GL_TRIANGLES);
	glColor3f(1, 0, 0);  /* Red */
	glVertex2f(-0.8, 0.8);
	glColor3f(0, 1, 0);  /* Green */
	glVertex2f(0.8, 0.8);
	glColor3f(0, 0, 1);  /* Blue */
	glVertex2f(0.0, -0.8);
	glEnd();
	cgGLDisableProfile(myCgVertexProfile);
	checkForCgError("disabling vertex profile");
	cgGLDisableProfile(myCgFragmentProfile);
	checkForCgError("disabling fragment profile");
	glutSwapBuffers();
}
int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(400,400);
	glutCreateWindow("Cg Test 4");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
```
运行结果：（按c键可以切换是否启用Cg加速）
![](https://img-my.csdn.net/uploads/201204/18/1334752538_8392.jpg)
最后列出两篇好的参考文章：
[http://www.owlei.com/DancingWind/Course/Tutorial_47.htm](http://www.owlei.com/DancingWind/Course/Tutorial_47.htm)
[http://blog.csdn.net/psophia/article/details/6104508](http://blog.csdn.net/psophia/article/details/6104508)
其次可以参考Cg Users Manual和The Cg Tutorial，nVidia的官方网址上面有。
