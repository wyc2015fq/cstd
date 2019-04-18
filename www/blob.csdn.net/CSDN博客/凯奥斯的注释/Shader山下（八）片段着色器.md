# Shader山下（八）片段着色器 - 凯奥斯的注释 - CSDN博客





2016年10月11日 13:29:50[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1219
所属专栏：[Shader山下](https://blog.csdn.net/column/details/12981.html)









# Shader可以分为两类：

# 表面着色器（Surface Shader） - 为你做了大部分的工作，只需要简单的技巧即可实现很多不错的效果，但也显示了部分功能。

# 片段着色器（Fragment Shader） - 可以做的事情更多，但是也比较难写。使用片段着色器的主要目的是可以在比较低的层级上进行更复杂（或者针对目标设备更高效）的开发。

一个最简单的片段着色器：


```cpp
Shader "Custom/Frag/Fragment" {
	Properties {
		_MainTex ("Albedo (RGB)", 2D) = "white" {}
		_MainTint ("Color", Color) = (1,1,1,1)
	}
	SubShader {
		Tags { "RenderType"="Opaque" }
		LOD 200

		Pass
		{
			CGPROGRAM
			#pragma vertex vert_img                             //1
			#pragma fragment frag                               //2
			#pragma fragmentoption ARB_precision_hint_fastest   //3
			#include "UnityCG.cginc"                            //4

			uniform sampler2D _MainTex;
			fixed4 _MainTint;

			fixed4 frag(v2f_img i) : COLOR                      //5
			{
				fixed4 renderTex = tex2D(_MainTex, i.uv) * _MainTint;
				return renderTex;
			}
			ENDCG
		}
	}
	FallBack "Diffuse"
}
```

用注释加了编号，方便讲解。

1处表示使用了方法名为vert_img的顶点着色器，我们可以在UnityCG.cginc文件中找到这个方法：


```cpp
v2f_img vert_img( appdata_img v )
{
	v2f_img o;
	o.pos = mul (UNITY_MATRIX_MVP, v.vertex);
	o.uv = v.texcoord;
	return o;
}
```

返回值类型：


```cpp
struct v2f_img
{
	float4 pos : SV_POSITION;
	half2 uv : TEXCOORD0;
};
```

输入参数值类型：


```cpp
struct appdata_img
{
	float4 vertex : POSITION;
	half2 texcoord : TEXCOORD0;
};
```

这个方法很简单，只是将顶点v的坐标与UNITY_MATRIX_MVP矩阵做了乘法，这个矩阵表示当前模型视图投影矩阵，它可以将顶点转换到屏幕坐标。




2处表示使用了方法名为frag的片段着色器。

3处表示使用了ARB_precision_hint_fastest这个选项，因为这我们要使用低精度来提升片段着色器的运行速度。

4处表示引用了UnityCG.cginc这个文件，这样我们才可以使用vert_img方法和v2f_img结构。

5处声明了片段着色器方法，输入为v2f_img类型，输出为fixed4形式的COLOR类型。而方法内容实现很简单，只是使用tex2D取出_MainTex上对应uv坐标的色彩值，再与_MainTint相乘，最后返回。




了解片段着色器的构成之后，我们就可以实现一个简单的转换灰度图的shader。


```cpp
Shader "Custom/Frag/GrayScale" {
	Properties {
		_MainTex ("Albedo (RGB)", 2D) = "white" {}
		_Luminosity ("GrayScale Amount", Range(0, 1)) = 1
	}
	SubShader {
		Tags { "RenderType"="Opaque" }
		LOD 200

		Pass
		{
			CGPROGRAM
			#pragma vertex vert_img
			#pragma fragment frag
			#pragma fragmentoption ARB_precision_hint_fastest
			#include "UnityCG.cginc"

			uniform sampler2D _MainTex;
			fixed _Luminosity;

			fixed4 frag(v2f_img i) : COLOR
			{
				fixed4 renderTex = tex2D(_MainTex, i.uv);
				half lum = 0.299 * renderTex.r + 0.587 * renderTex.g + 0.114 * renderTex.b;
				fixed4 finalColor = lerp(renderTex, lum, _Luminosity);
				return finalColor;
			}
			ENDCG
		}
	}
	FallBack "Diffuse"
}
```

我们声明了一个_Luminosity变量用来表示灰度程度。

frag里面，我们使用了灰度转换的经典公式：Gray=0.299*r+0.587*g+0.114*b。

然后根据_Luminosity做插值，来实现不同的灰度程度。




效果：

GrayScaleAmount
 1

↓↓↓↓↓↓

![](https://img-blog.csdn.net/20161011132608346)


GrayScaleAmount
 2


↓↓↓↓↓↓


![](https://img-blog.csdn.net/20161011132620638)


GrayScaleAmount
 3


↓↓↓↓↓↓


![](https://img-blog.csdn.net/20161011132632951)







