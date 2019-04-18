# 【GPU编程】The Cg Runtime:OpenGL中调用Cg程序 - 江南烟雨 - CSDN博客
2012年01月10日 14:54:49[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：3111
所属专栏：[](https://blog.csdn.net/column/details/.html)
原文：《The Cg Tutorial》（Cg手册）：Appendix B:The Cg Runtime,Page195
很多地方可能翻译得不好，还请大家见谅、指教！
注意：我省略了其中的Direct3D的部分，因为本人现在正使用OpenGL开发。
B.1 什么是Cg Runtime?
Cg程序支持运行在GPU上的程序，但是它们需要应用程序的支持才能渲染图像。
为了把Cg程序和应用程序链接起来，你必须做两件事情：
(1)把程序编译为不同的版本。这一步把你的Cg程序转化成与应用程序的3D编程接口和硬件兼容的形式。
(2)把程序和应用程序链接起来。这一步使得应用程序能够配置程序来执行，并且能够赋予不同的以及全局变量参数。
你可以在任何想使用这些操作的时候来选择它。你可以在编译时刻使用它们，此时应用程序已经被编译成可执行的形式，或者是在运行时刻使用它们，此时应用程序已经被执行。Cg Runtime是这样一些应用程序编写接口的集合：它们使得一个应用程序能够在运行时刻编译和链接Cg程序。
B.2 为什么要使用Cg Runtime？
**B.2.1 面向未来**
大多数应用程序需要在不同性能的GPU上运行，所以这些应用程序需要在不同的profile（一个Cg profile是被特定图形硬件和API所支持的Cg语言子集）上运行。如果一个应用程序在编译时刻预编译了它的Cg程序，它必须为每个程序保存适用于每个profile的预编译版本。对于一个使用了很多Cg程序的应用程序来说十分累赘。更糟糕的是，这些Cg程序在运行时刻不能灵活更改。通过预编译的方法，一个应用程序牺牲了未来的编译器可能提供的优化功能。
**B.2.2 去除依赖性的问题**
如果你把已经已经编译的Cg程序和一个应用程序链接起来，应用程序就和编译结果绑定了，尤其是和编译器如何分配参数有关。应用程序不得不通过使用Cg编译器输出的硬件寄存器名来参照Cg程序输入参数。
1. 在Cg程序中，不查看编译器输出结果寄存器名字难以和Cg程序中的相关有意义的名字关联起来。
2. 每当Cg程序、Cg编译器或者编译的profile改变时寄存器分配都可能发生变化。这就意味着你不得不每次更新应用程序，这是很不方便的。
**B.2.3 输入参数管理**
Cg runtime也提供了管理Cg程序输入参数的功能。特别的，它使得处理比如数组和矩阵这样的数据类型更加容易。
这些传统的函数也包括了那些必要的3D API调用，它们能够使得代码更短，减少程序员犯错误的次数。
B.3 Cg Runtime是如何工作的？
下图显示了组成Cg runtime API的三个库：
![](http://hi.csdn.net/attachment/201201/10/0_1326165125m7cR.gif)
(1)一个函数和结构的核心集封装了runtime的独立于3D API的功能。
(2)建立于核心集之上的面向OpenGL的函数集
(3)建立于核心集之上的面向Direct3D的函数集
为了使应用程序编写者更加方便的编写程序，OpenGL和Direct3D库都能够采用他们各自API的规范和数据结构类型。你只需要把你的应用程序所使用的3D API和针对特定3D API的Cg runtime库链接起来。这样，大多数应用程序能够使用OpenGL或者Direct3D Cg runtime库。
**B.3.1 头文件**
下面显示了如何在你的C或者C++程序中包含核心Cg runtime API：
#include <Cg/cg.h>
下面显示了如何包含针对OpenGL的Cg runtime API：
#include <Cg/cgGL.h>
**B.3.2 创建上下文**
一个上下文是用来存放Cg程序的容器。它存放你加载的Cg程序以及他们共享的数据。
下面显示了如何创建上下文：
CGcontext context = cgCreateContext();
**B.3.3 编译程序**
使用cgCreateProgram函数通过把它添加到一个上下文中来编译一个Cg程序：
```cpp
CGprogram program = 
cgCreateProgramFromFile(
						context,
						CG_SOURCE,
						programString,
						profile,
						"main",
						args
						);
```
参数CG_SOURCE表明接下来的参数programString是包含了Cg源代码的字节，而不是预编译代码。Cg runtime也允许你通过使用CG_OBJECT而不是CG_SOURCE来从已经编译的代码（也称目标代码）创建一个程序。
profile指明了这个程序将要编译成的profile。
"main"指明了你的程序的入口函数名。
args是这样的一个字符串，它提供选项给编译器。
**B.3.4 加载程序**
 在你编译完程序之后，你需要将结果目标代码传递给你使用的3D API。这时，你需要调用Cg runtime的特定的3D API函数。
在OpenGL中，你可以这样加载一个程序：
cgGLLoadProgram(program);
**B.3.5 更改程序参数**
runtime允许你更改你的程序的参数值。第一步是获得参数的句柄。
CGparameter myParameter = cgGetNamedParameter(program,"myParameter");
myParameter是出现在程序源代码中的参数名。
第二步就是设置参数的值，所需使用的函数取决于参数类型。
比如OpenGL中的一个例子：
cgGLSetParameter4fv(myParameter,value);
这个函数把值value(一个包含4个浮点类型的数组)赋给参数myParameter。
**B.3.6 执行程序**
在你执行一个程序之前，你必须使其相关的profile可用。比如：
cgGLEnableProfile(CG_PROFILE_ARBFP1);
接下来。你必须把程序与当前3D API状态绑定起来。这就意味着它将要在接下来的每个顶点(假设是顶点程序)以及每个片段(假设是片段程序)的绘制调用中执行。
下面显示了在OpenGL中如何绑定一个程序：
cgGLBindProgram(program);
在同一时刻你只能为特定的profile绑定一个顶点及片段程序。因此，只有别的顶点程序没有被绑定时，这个顶点程序才会被执行。对于片段程序也是一样。
在OpenGL中，禁用profile使用以下调用：
cgGLDisableProfile(CG_PROFILE_ARBFP1);
**B.3.7 释放资源**
如果你的应用程序不再需要Cg程序，那么释放由Cg runtime为程序维护的资源是一个好的编程习惯。
释放为单一程序分配的资源，使用下列函数调用：
cgDestroyProgram(program);
释放为一个上下文所分配的所以资源，使用下列函数调用：
cgDestroyContext(context);
注意：destroy一个上下文意味着释放了它包含的所以程序。
**B.3.8 错误捕获**
核心Cg runtime通过设置一个包含错误码的全局变量来报告错误。你能够以下列方式查询它以及相关错误字符串：
CGerror error = cgGetError();
const char *errorString = cgGetErrorString(error);
每次一个错误发生时，核心库会可选的调用一个由应用程序提供的回调函数。这个回调函数经常会调用cgGetError:
调用特定3D API也可能会产生关于特定API的错误。对于OpenGL Cg runtime库，使用glGetError来检测。
以这种方式，很多有用的信息都会被输出到调试输出控制台上。
```cpp
void MyErrorCallbask()
{
	const char* errorString = cgGetErrorString(cgGetError());
	printf(logfile,"Cg error:%s",errorString);
}
cgSetErrorCallback(MyErrorCallbask);
```
最后粘贴一个实例：
C3E1v_green.cg：
```cpp
struct C3E1v_Output{
	float4 position:POSITION;
	float3 color:COLOR;
};
C3E1v_Output C3E1v_green(float2 position:POSITION,
											uniform float3 constantColor)
{
	C3E1v_Output OUT;
	OUT.position = float4(position,0,1);
	OUT.color = constantColor;
	return OUT;
}
```
C2E2f_passthru.cg：
```cpp
struct C2E2f_Output{
	float4 color:COLOR;
};
C2E2f_Output C2E2f_passthru(float4 color:COLOR)
{
	C2E2f_Output OUT;
	OUT.color = color;
	return OUT;
}
```
02_vertex_and_fragment_program.cpp：
```cpp
#pragma comment( lib, "cg.lib" )
#pragma comment( lib, "cgGL.lib" )
#include <iostream>
#include <stdlib.h>
#include <cmath>
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>
using namespace std;
static CGcontext myCgContext;
static CGprofile myCgVertexProfile;
static CGprofile myCgFragmenrProfile;
static CGprogram myCgVertexProgram;
static CGprogram myCgfragmentProgram;
static const char *myProgramName = "02_vertex_and_fragment_program",
						*myVertexProgramFileName = "C3E1v_green.cg",
						*myVertexProgramName = "C3E1v_green",
						*myFragmenrProgramFileName = "C2E2f_passthru.cg",
						*myFragmentProgramName = "C2E2f_passthru";
static CGparameter myCgVertexParam_constantColor;
static void checkForError(const char* situation)
{
	CGerror error;
	const char *string = cgGetLastErrorString(&error);
	if (error != CG_NO_ERROR)
	{
		cout<<myProgramName<<":"<<situation<<":"<<string<<endl;
		if (error == CG_COMPILER_ERROR)
		{
			cout<<cgGetLastListing(myCgContext)<<endl;
		}
		exit(1);
	}
}
static void display();
static void keyboard(unsigned char c,int x,int y);
int main(int argc,char** argv)
{
	glutInitWindowSize(400,400);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInit(&argc,argv);
	glutCreateWindow(myProgramName);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glClearColor(0.1,0.3,0.6,0.0);
	myCgContext = cgCreateContext();
	checkForError("creating context");
	cgGLSetDebugMode(CG_FALSE);
	cgSetParameterSettingMode(myCgContext,CG_DEFERRED_PARAMETER_SETTING);
	myCgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
	cgGLSetOptimalOptions(myCgVertexProfile);
	checkForError("selecting vertex profile");
	myCgVertexProgram = cgCreateProgramFromFile(
		myCgContext,
		CG_SOURCE,
		myVertexProgramFileName,
		myCgVertexProfile,
		myVertexProgramName,
		NULL
		);
	checkForError("creating vertex program from file");
	cgGLLoadProgram(myCgVertexProgram);
	checkForError("loading vertex program");
	myCgVertexParam_constantColor = cgGetNamedParameter(
		myCgVertexProgram,"constantColor");
	checkForError("could not get constantColor parameter");
	myCgFragmenrProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
	cgGLSetOptimalOptions(myCgFragmenrProfile);
	checkForError("selecting fragment profile");
	myCgfragmentProgram = cgCreateProgramFromFile(
		myCgContext,
		CG_SOURCE,
		myFragmenrProgramFileName,
		myCgFragmenrProfile,
		myFragmentProgramName,
		NULL
		);
	checkForError("creating fragment program from file");
	cgGLLoadProgram(myCgfragmentProgram);
	checkForError("loading fragment program");
	glutMainLoop();
	return 0;
}
static void drawStar(float x, float y,
					 int starPoints, float R, float r)
{
	int i;
	double piOverStarPoints = 3.14159 / starPoints,
		angle = 0.0;
	cgUpdateProgramParameters(myCgVertexProgram);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);  /* Center of star */
	/* Emit exterior vertices for star's points. */
	for (i=0; i<starPoints; i++) {
		glVertex2f(x + R*cos(angle), y + R*sin(angle));
		angle += piOverStarPoints;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
		angle += piOverStarPoints;
	}
	/* End by repeating first exterior vertex of star. */
	angle = 0;
	glVertex2f(x + R*cos(angle), y + R*sin(angle));
	glEnd();
}
static void drawStars(void)
{
	/*                     star    outer   inner  */
	/*        x      y     Points  radius  radius */
	/*       =====  =====  ======  ======  ====== */
	const float green[3] = {0.2,0.8,0.3};
	cgSetParameter3fv(myCgVertexParam_constantColor,green);
	drawStar(-0.1,   0,    5,      0.5,    0.2);
	drawStar(-0.84,  0.1,  5,      0.3,    0.12);
	drawStar( 0.92, -0.5,  5,      0.25,   0.11);
	cgSetParameter3f(myCgVertexParam_constantColor,
		0.7,0.1,0.1);
	drawStar( 0.3,   0.97, 5,      0.3,    0.1);
	drawStar( 0.94,  0.3,  5,      0.5,    0.2);
	drawStar(-0.97, -0.8,  5,      0.6,    0.2);
}
static void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cgGLBindProgram(myCgVertexProgram);
	checkForError("Binding vertex program");
	cgGLEnableProfile(myCgVertexProfile);
	checkForError("enabling vertex profile");
	cgGLBindProgram(myCgfragmentProgram);
	checkForError("Binding fragment program");
	cgGLEnableProfile(myCgFragmenrProfile);
	checkForError("enabling fragment profile");
	drawStars();
	cgGLDisableProfile(myCgVertexProfile);
	checkForError("disabling vertex profile");
	cgGLDisableProfile(myCgFragmenrProfile);
	checkForError("disabling fragment profile");
	glutSwapBuffers();
}
static void keyboard(unsigned char c,int x,int y)
{
	switch(c)
	{
		case 27:
			cgDestroyProgram(myCgVertexProgram);
			cgDestroyProgram(myCgfragmentProgram);
			cgDestroyContext(myCgContext);
			exit(0);
			break;
	}
}
```
运行结果：
![](http://hi.csdn.net/attachment/201201/10/0_1326182859d0R7.gif)
