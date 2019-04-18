# Shader山下（一）第一个Shader - 凯奥斯的注释 - CSDN博客





2016年10月06日 18:08:49[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：3134
所属专栏：[Shader山下](https://blog.csdn.net/column/details/12981.html)









# 学习Unity3D就得学Shader，然而新手学习Shader往往不知道从何处下手，没有找对门，谈何入门。

本系列主要介绍Shader的一些基础知识，希望可以带你找对门，然后入得了门。

![](https://img-blog.csdn.net/20161006162629812)



（本文作者也尚在山脚下，如果走错了路，欢迎指正。）





本系列专栏地址：[Shader山下](http://blog.csdn.net/column/details/12981.html)

附赠Unity内建Shader[下载地址](https://unity3d.com/get-unity/download/archive)










那么我们就从编写第一个Shader开始。

首先，我们在Unity里创建一个Shader。

![](https://img-blog.csdn.net/20161006163440246)


双击打开这个shader，shader里的内容我们先不管，直接清空，贴入下面这段代码：



```cpp
Shader "Custom/BasicDiffuse" {
	Properties {
		_MainTex ("Albedo (RGB)", 2D) = "white" {}
	}
	SubShader {
		Tags { "RenderType"="Opaque" }
		LOD 200
		
		CGPROGRAM
		#pragma surface surf Lambert

		sampler2D _MainTex;

		struct Input {
			float2 uv_MainTex;
		};

		void surf (Input IN, inout SurfaceOutput o) {
			fixed4 c = tex2D (_MainTex, IN.uv_MainTex);
			o.Albedo = c.rgb;
			o.Alpha = c.a;
		}
		ENDCG
	}
	FallBack "Diffuse"
}
```

这是一个最简单的shader。


**Properties**里面包含的是shader的属性。以_MainTex为例， _MainTex表示变量名，Albedo(RGB)是在编辑器里显示的名称，2D是它的类型，表示它是一个纹理，white是默认值。




表面着色器的属性类型：



Range (min, max)
创建 Float 属性，以滑动条的形式便于在最大值和最小值之间进行调节创建色块，

Color

可以在 Inspector 面板上通过拾色器获取颜色值 =
 (float,float,float,float)

2D

创建纹理属性，允许直接拖曳一个纹理

Rect

创建一个非 2 次方的纹理属性，作为 2D
 GUI 元素


Cube

在 Inspector 面板上创建一个立方贴图属性，允许用户直接拖曳立方贴图作为着色器属性


Float

在 Inspector 面板上创建一个非滑动条的 float 属性


Vector



创建一个拥有 4个float值的属性，可以用于标记方向或颜色 







**SubShader**是子着色器，一个着色器中可以包含多个SubShader。子着色器这是代码的主体，计算着色的时候，平台会按顺序选择一个可以使用的子着色器进行执行，如果所有的子着色器都无法使用，则会执行最后**FallBack**里指定的着色器。

我们来看SubShader的主体：

***Tags***（标签）标记了着色器的一些特性。

常用的Tag有：

RenderType：渲染类型，常用就是Opaque（不透明）和Transparent（透明）。

IgnoreProjector：是否忽略投影器，True or False。

ForceNoShadowCasting：是否强制无阴影，True or False。

Queue：渲染队列，内置值Background=1000,Geometry=2000,AlphaTest=2450,Transparent=3000,Overlay=4000，但是并不限于这些值，你可以填写自己的值，或者写成"Queue"="Transparent+10"也是可以的。

***LOD***是Level of Details的缩写，表示着色器的细节层级，高于Unity的最大LOD（Quality
 Settings里设置）的shader将不可用。在调低画质时，可以根据这个值舍弃掉一部分shader。




紧接着是***CGPROGRAM***，它与***ENDCG***对应，表明在二者范围内是一段Cg(C
 for graphics)代码。

***#pragma*** surface surf Lambert


这一行表明我们使用的是一个表面着色器，方法名称是surf，光照模型是Lambert。

然后是

***sampler2D*** _MainTex;


sampler2D对应于Properties里面的2D，是2D贴图的数据结构。而_MainTex也对应于Properties里面的_MainTex，保存了编辑器（或者代码）里设置的贴图。二者必须是同名，才能将贴图数据链接起来。简而言之，下面的_MainTex是上面的_MainTex在Cg代码里的代理。

然后是一个结构（***struct***）定义：



```cpp
struct Input {
			float2 uv_MainTex;
		};
```
这个结构是为surf方法定义了输入参数的数据结构。


float2表示这是一个二维的浮点型坐标。

其他的内置类型还包括：

half：半精度浮点型，范围[-60000,60000]

fixed：低精度定点型，范围[-2,2]

int：整型

bool：布尔型

sample*d：纹理类型

uv_MainTex表示_MainTex的纹理坐标（参考百度百科[UV坐标](http://baike.baidu.com/link?url=Ud19scY012C7WZnd-LDdi7etKQ8CbX9fAWGW6D4xOYle6lGyU5dgEzYPO_K5vyAOXeW22-EC68wd6JtCS6fzDa)），这是一种命名约定。

最后是***surf***方法：



```
void surf (Input IN, inout SurfaceOutput o) {
			fixed4 c = tex2D (_MainTex, IN.uv_MainTex);
			o.Albedo = c.rgb;
			o.Alpha = c.a;
		}
```

Input结构是我们在上面定义的。


SurfaceOutput的数据结构：



```
struct SurfaceOutput {
    half3 Albedo;     //像素的颜色
    half3 Normal;     //像素的法向值
    half3 Emission;   //像素的发散颜色
    half Specular;    //像素的镜面高光
    half Gloss;       //像素的发光强度
    half Alpha;       //像素的透明度
};
```






surf方法主体第一行：


`fixed4 c = tex2D (_MainTex, IN.uv_MainTex);`
使用tex2D方法从_MainTex里面取出指定纹理坐标（IN.uv_MainTex）的色彩值。


取出来色彩值之后，很简单，将c的rgb分量赋值给输出o的Albedo，把c的a分量赋值给输出o的Alpha。




那么我们已经可以读懂一个最简单的shader了，那么是不是应该自己再写一个：



```cpp
Shader "Custom/TestColor" {
	Properties {
		_Color ("Color", Color) = (1,1,1,1)
	}
	SubShader {
		Tags { "RenderType"="Opaque" }
		LOD 200
		
		CGPROGRAM
		#pragma surface surf Lambert

		fixed4 _Color;

		struct Input {
			float2 uv_MainTex;
		};

		void surf (Input IN, inout SurfaceOutput o) {
			o.Albedo = _Color.rgb;
			o.Alpha = _Color.a;
		}
		ENDCG
	}
	FallBack "Diffuse"
}
```

在Unity编辑器里新建一个材质，将材质的shader设置成我们新写的TestColor。然后在场景里面加入一个Cube，并将Cube的材质设置为我们新建的材质。


这样我们就完成了一个最简单的单色材质，可以通过调节材质的颜色来调节Cube的颜色了。



