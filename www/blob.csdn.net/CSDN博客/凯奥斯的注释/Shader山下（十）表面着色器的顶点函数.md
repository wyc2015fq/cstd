# Shader山下（十）表面着色器的顶点函数 - 凯奥斯的注释 - CSDN博客





2016年10月12日 08:59:30[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：2139
所属专栏：[Shader山下](https://blog.csdn.net/column/details/12981.html)









# 对于每个顶点，GPU都会执行一次顶点函数，顶点函数的任务就是从3D局部空间或者transform得到的顶点转换到2D屏幕。我们可以使用顶点函数修改顶点的一些参数，例如顶点位置、顶点颜色和UV坐标。修改过这些参数之后，会将它们传递给surf函数。

首先我们需要创建一份最简单的漫反射shader：



```cpp
Shader "Custom/Vertex/Base" {
	Properties {
		_MainTex ("Albedo (RGB)", 2D) = "white" {}
	}
	SubShader {
		Tags { "RenderType"="Opaque"}
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

修改pragma这一行：


```cpp
#pragma surface surf Lambert vertex:vert
```

表明我们要使用名称为vert的顶点方法。
Input中增加：



```cpp
float4 vertColor;
```

实现vert方法：


```cpp
void vert(inout appdata_full v, out Input o)
		{
			UNITY_INITIALIZE_OUTPUT(Input, o);
			o.vertColor = v.color;
		}
```

我们注意到这个方法的输出o与surf方法的输入IN类型相同，其实就是在顶点为surf方法赋值了输入数据。
如果注释掉UNITY_INITIALIZE_OUTPUT这一行，会报错：



```cpp
'vert': output parameter 'o' not completely initialized at line 25 (on glcore)
```

因为Input里面的uv_MainTex还没有初始化。
我们可以看一看appdata_full结构，在UnityCG.cginc里面：



```cpp
struct appdata_full {
	float4 vertex : POSITION;
	float4 tangent : TANGENT;
	float3 normal : NORMAL;
	float4 texcoord : TEXCOORD0;
	float4 texcoord1 : TEXCOORD1;
	float4 texcoord2 : TEXCOORD2;
	float4 texcoord3 : TEXCOORD3;
#if defined(SHADER_API_XBOX360)
	half4 texcoord4 : TEXCOORD4;
	half4 texcoord5 : TEXCOORD5;
#endif
	fixed4 color : COLOR;
};
```

包含了坐标、切线、法线、uv、uv1、uv2、uv3、（uv4、uv5、）顶点颜色。
所以我们也可以把vert方法这样写：



```cpp
void vert(inout appdata_full v, out Input o)
		{
			o.uv_MainTex = v.texcoord.xy;
			o.vertColor = v.color;
		}
```

我们不仅可以修改o，还可以修改作为输入输出值的v。例如我们在Properties里添加：


```cpp
_NormalYScale ("Normal Y Scale", Range(-2, 2)) = 1
```

SubShader里添加：


```cpp
fixed _NormalYScale;
```
并在vert方法里添加：


```cpp
v.normal = half3(v.normal.x, v.normal.y * _NormalYScale, v.normal.z);
```

原效果（Normal Y Scale = 1）：
![](https://img-blog.csdn.net/20161012085147626)



Normal Y Scale = 2：

![](https://img-blog.csdn.net/20161012085219272)


Normal Y Scale = 0：


![](https://img-blog.csdn.net/20161012085316814)


Normal Y Scale = -1：


![](https://img-blog.csdn.net/20161012085326539)


Normal Y Scale = -2：


![](https://img-blog.csdn.net/20161012085333617)




