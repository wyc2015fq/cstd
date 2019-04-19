# 看得让人DT的GLSL - 逍遥剑客 - CSDN博客
2009年10月16日 21:15:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：4201
GLES2.0封装到shader和vbo结合的部分, 蒙了
vertex layout(或叫vertex declaration)要一个一个element的绑定到一个shader变量上(GL叫attribute), 也就是相当于HLSL的VS_INPUT结构体吧.
问题是, GLSL里没有"semantic", 然后问题就来了
举个例子:
一个简单的顶点结构:
struct Vertex
{
    float3 position;
    float3 normal;
    float2 uv;
}
在DX中可以通过指定vetex declaration来绑定到指定寄存器, HLSL里通过语义来标明当前输入变量是对应哪个寄存器的:
struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal   : NORMAL;
    float2 uv       : TEXCOORD;
}
但是对于GLSL, 它没有vertex declaration这么个东西! 怪不得大家都用CG...无奈手机上用不了CG, 还是要面对这个问题.
起初我是设想的把每种vertex element对应一个固定的attribute名, 如a_position0, a_normal0, a_color1之类, 然后用程序自动绑定. 写shader的时候只能使用程序里规定的attribute名字. 写到glBindAttribLocation时, 问题又来了:
glBindAttribLocation之后要glLinkProgram才有效果~
也就是说, 每个effect(vs + ps)要在link之前把所有参数全部手动绑定一遍. 但对于vertex layout来说不太现实. 难道还要自己用XML定义一下所有参数? 昏倒
想来想去, 好像只有OGRE有用GLSL, 查了一下, 我原本想的没有错, 它也是用固定attribute名来绑定element的:
	//  a  builtin				custom attrib name
	// ----------------------------------------------
	//	0  gl_Vertex			vertex
	//  1  n/a					blendWeights		
	//	2  gl_Normal			normal
	//	3  gl_Color				colour
	//	4  gl_SecondaryColor	secondary_colour
	//	5  gl_FogCoord			fog_coord
	//  7  n/a					blendIndices
	//	8  gl_MultiTexCoord0	uv0
	//	9  gl_MultiTexCoord1	uv1
	//	10 gl_MultiTexCoord2	uv2
	//	11 gl_MultiTexCoord3	uv3
	//	12 gl_MultiTexCoord4	uv4
	//	13 gl_MultiTexCoord5	uv5
	//	14 gl_MultiTexCoord6	uv6, tangent
	//	15 gl_MultiTexCoord7	uv7, binormal
 但是它没有在乎vertexlayout究竟是什么样的, 一鼓脑的全绑定上了
这样的话, glEnableVertexAttribArray的index就需要映射一下了...
嗯...shader里多声明几个不使用的变量冒似没有什么问题, 就这么干了~
