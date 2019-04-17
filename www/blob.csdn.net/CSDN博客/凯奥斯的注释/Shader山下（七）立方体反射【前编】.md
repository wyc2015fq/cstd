# Shader山下（七）立方体反射【前编】 - 凯奥斯的注释 - CSDN博客





2016年10月10日 09:09:09[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：666标签：[Unity																[Shader																[反射																[Reflection																[Mask](https://so.csdn.net/so/search/s.do?q=Mask&t=blog)
个人分类：[Shader](https://blog.csdn.net/ecidevilin/article/category/6448925)

所属专栏：[Shader山下](https://blog.csdn.net/column/details/12981.html)](https://so.csdn.net/so/search/s.do?q=Reflection&t=blog)




# 反射着色器可以获取外界的场景信息，并将它们反映到材质表面来模拟周围环境。

本文就要实现一个模拟反射的着色器，这里我们需要使用Cubemap来完成这个功能。

首先创建一个Cubemap：

![](https://img-blog.csdn.net/20161010084334015)


然后编辑Cubemap：

![](https://img-blog.csdn.net/20161010084456076)


这里建议先使用Unity自带的Default-Particle先看看效果。

接着我们开始编写shader：



```
Shader "Custom/Reflection/Base" {
	Properties {
		_MainTex ("Albedo (RGB)", 2D) = "white" {}
		_MainTint ("Main Tint", Color) = (1,1,1,1)
		_CubeMap ("CubeMap", CUBE) = "" {}
		_ReflAmount ("Reflection Amount", Range(0.01, 1)) = 0.5
	}
	SubShader {
		Tags { "RenderType"="Opaque"}
		LOD 200
		
		CGPROGRAM
		#pragma surface surf Lambert

		sampler2D _MainTex;
		fixed4 _MainTint;
		samplerCUBE _CubeMap;
		fixed _ReflAmount;

		struct Input {
			float2 uv_MainTex;
			float3 worldRefl;
		};

		void surf (Input IN, inout SurfaceOutput o) {
			fixed4 c = tex2D (_MainTex, IN.uv_MainTex) * _MainTint;
			o.Emission = texCUBE(_CubeMap, IN.worldRefl).rgb * _ReflAmount;
			o.Albedo = c.rgb;
			o.Alpha = c.a;
		}

		ENDCG
	}
	FallBack "Diffuse"
}
```
代码很简单。需要注意几个点：


1、Input里面增加了worldRefl变量，表示世界空间的反射向量。如图所示：

![](https://img-blog.csdn.net/20161010084945578)

worldRefl = viewDir - 2.0 * normal * dot(viewDir, normal)

2、使用texCUBE获取worldRefl位置上的Cubemap上的色彩值，然后乘以系数传递给o.Emission（自发光）。



将shader和Cubemap应用到场景物体上就会获得这样的效果：

![](https://img-blog.csdn.net/20161010085701363)


接着我们可以替换Cubemap上的图片，来感受反射的不同，例如把Right替换成：

![](https://img-blog.csdn.net/20161010085811393)


就会获得：

![](https://img-blog.csdn.net/20161010085844909)





接着我们为反射增加遮罩。

首先在Properties里添加：


`		_ReflMask ("Reflection Mask", 2D) = "" {}`SubShader里添加：
`sampler2D _ReflMask;`修改surf方法：




```
void surf (Input IN, inout SurfaceOutput o) {
			fixed4 c = tex2D (_MainTex, IN.uv_MainTex) * _MainTint;
			fixed3 r = texCUBE(_CubeMap, IN.worldRefl).rgb;
			fixed4 m = tex2D (_ReflMask, IN.uv_MainTex);

			o.Emission = (r * m.r) * _ReflAmount;

			o.Albedo = c.rgb;
			o.Alpha = c.a;
		}
```


修改很少，只是获取了mask里面的色彩值，与Cubemap上的色彩值相乘。




可以使用这张图片作为遮罩：

![](https://img-blog.csdn.net/20161010090442630)


得到效果：

![](https://img-blog.csdn.net/20161010090511990)





其他应用：

[Shader山下（九）立方体反射【后编】](http://blog.csdn.net/ecidevilin/article/details/52791709)](https://so.csdn.net/so/search/s.do?q=反射&t=blog)](https://so.csdn.net/so/search/s.do?q=Shader&t=blog)](https://so.csdn.net/so/search/s.do?q=Unity&t=blog)




