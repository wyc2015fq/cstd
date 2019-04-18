# Shader山下（九）立方体反射【后编】 - 凯奥斯的注释 - CSDN博客





2016年10月11日 21:58:47[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1184
所属专栏：[Shader山下](https://blog.csdn.net/column/details/12981.html)









# [Shader山下（七）立方体反射【前编】](http://blog.csdn.net/ecidevilin/article/details/52774292)里介绍了如何使用Cubemap来模拟环境反射，本文就再介绍一下其他两个应用。

## 首先介绍立方体反射与法线贴图（[Shader山下（四）法线贴图](http://blog.csdn.net/ecidevilin/article/details/52753654)）的结合。

我们先按照前文的方法，制作一个Cubemap。

然后复制一个最简单的立方体反射shader：



```cpp
Shader "Custom/Reflection/Normal" {
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
Properties里增加：


```cpp
_NormalMap ("Normal Map", 2D) = "bump" {}
```

SubShader里增加：


```cpp
sampler2D _NormalMap;
```

修改Input结构：


```cpp
struct Input {
			float2 uv_MainTex;
			float2 uv_NormalMap;
			float3 worldRefl;
			INTERNAL_DATA
		};
```

除了增加了uv_NormalMap之外，我们还增加了一个宏定义INTERNAL_DATA，我们稍后会讲到。
修改surf方法：



```cpp
void surf (Input IN, inout SurfaceOutput o) {
			fixed4 c = tex2D (_MainTex, IN.uv_MainTex) * _MainTint;
			fixed3 n = UnpackNormal(tex2D(_NormalMap, IN.uv_NormalMap)).rgb;

			o.Normal = n;
			o.Emission = texCUBE(_CubeMap, WorldReflectionVector(IN, o.Normal)).rgb * _ReflAmount;
			o.Albedo = c.rgb;
			o.Alpha = c.a;
		}
```

取出法线向量，作为WorldReflectionVector方法的参数计算反射向量。



这时候，如果我们把INTERNAL_DATA注释掉的话，Unity会报错：


`Shader error in 'Custom/Reflection/NormalMap': Surface shader Input structure needs INTERNAL_DATA for this WorldNormalVector or WorldReflectionVector usage at line 36 (on glcore)`
所以我们还是把INTERNAL_DATA解注，才能使用WorldReflectionVector方法。
挑选一个Normal Map作为输入，例如这张：


![](https://img-blog.csdn.net/20161011212356240)


得到效果：

![](https://img-blog.csdn.net/20161011212445694)


如果我们不修改o.Emission这一句，还是使用原来的值：



```cpp
//			o.Emission = texCUBE(_CubeMap, WorldReflectionVector(IN, o.Normal)).rgb * _ReflAmount;
			o.Emission = texCUBE(_CubeMap, IN.worldRefl).rgb * _ReflAmount;
```

就会得到这样的效果：
![](https://img-blog.csdn.net/20161011212630102)


因为法线不对应，所以无法形成发射效果。




## 我们还可以将立方体反射和镜面高光（[Shader山下（六）镜面高光](http://blog.csdn.net/ecidevilin/article/details/52763076)）结合起来。

我们还是从基础的立方体反射开始：



```cpp
Shader "Custom/Reflection/BlinnPhong" {
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

增加Properties：


```cpp
_RimPower ("Fresnel Falloff", Range(0.1, 3)) = 2
		_SpecColor ("Specular Color", Color) = (1,0,0,1)
		_SpecPower ("Specular Power", Range(0, 1)) = 0.2
```

SubShader添加变量：


```cpp
half _RimPower;
		fixed _SpecPower;
```

Input结构里增加视图向量：


```cpp
float3 viewDir;
```

修改surf方法：


```cpp
void surf (Input IN, inout SurfaceOutput o) {
			fixed4 c = tex2D (_MainTex, IN.uv_MainTex) * _MainTint;

			half rim = saturate(dot(o.Normal, normalize(IN.viewDir)));
			
			rim = 1.0 - rim;
			rim = pow(rim, _RimPower);

			o.Emission = texCUBE(_CubeMap, IN.worldRefl).rgb * _ReflAmount * rim;
			o.Specular = _SpecPower;
			o.Gloss = 1.0;
			o.Albedo = c.rgb;
			o.Alpha = c.a;
		}
```

我们先看看效果：
![](https://img-blog.csdn.net/20161011214404720)


如果注释掉这一行：



```cpp
rim = 1.0 - rim;
```

得到效果：
![](https://img-blog.csdn.net/20161011214734443)





我们知道，rim是计算了法线向量与视图向量之间的点积（数值越大表示接收到的视线越多），而1-rim就正好翻转了这个值，也就是说背向摄像机的面反射效果更明显。





