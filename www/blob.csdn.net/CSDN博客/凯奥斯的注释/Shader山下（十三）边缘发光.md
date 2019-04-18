# Shader山下（十三）边缘发光 - 凯奥斯的注释 - CSDN博客





2016年10月13日 21:36:20[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1614
所属专栏：[Shader山下](https://blog.csdn.net/column/details/12981.html)









# 在很多游戏中，都会让出现模型边缘发光的效果，本文就用表面着色器实现一个简单的边缘发光的效果。

首先我们需要一个最简单的漫反射表面着色器：



```cpp
Shader "Custom/TestUV" {
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
Properties里增加：


```cpp
_RimColor("Rim Color", Color) = (1,1,1,1)
		_RimPower("Rim Power", Range(0.5,8.0)) = 3.0
```

SubShader里增加：


```cpp
float4 _RimColor;
		float _RimPower;
```

一个是边缘发光的颜色，一个是幂。




Input结构里添加视图向量：



```cpp
float3 viewDir;
```


修改surf方法：


```cpp
void surf (Input IN, inout SurfaceOutput o) {
			fixed4 c = tex2D (_MainTex, IN.uv_MainTex);
			o.Albedo = c.rgb;
			o.Alpha = c.a;
			half rim = 1.0 - abs(dot (normalize(IN.viewDir), o.Normal));
			o.Emission = _RimColor.rgb * pow(rim, _RimPower);
	    }
```



重点是第四行，首先计算视图向量与法线向量的点积。我们知道点积值在[-1,1]之间，1表示法线与视图同向平行，0表示法线与视图垂直，-1表示法线与视图同向平行。当法线与视图垂直的时候也就意味着像素点平行于视图向量，也就是边缘位置。所以我们取点积的绝对值并用反转，获得rim。rim越大越靠近边缘。

然后根据rim值为像素添加自发光即可。

效果：

![](https://img-blog.csdn.net/20161013212427007)

![](https://img-blog.csdn.net/20161013212617742)




