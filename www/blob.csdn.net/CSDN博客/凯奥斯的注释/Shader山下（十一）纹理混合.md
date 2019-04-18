# Shader山下（十一）纹理混合 - 凯奥斯的注释 - CSDN博客





2016年10月12日 13:57:18[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1673
所属专栏：[Shader山下](https://blog.csdn.net/column/details/12981.html)









# [Shader山下（八）片段着色器](http://blog.csdn.net/ecidevilin/article/details/52787052)介绍了如何编写片段着色器，本文就使用它实现纹理混合效果。

首先需要一份最简单的片段着色器：



```cpp
Shader "Custom/Frag/Blend" {
	Properties {
		_MainTex ("Albedo (RGB)", 2D) = "white" {}
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

			fixed4 frag(v2f_img i) : COLOR                      
			{
				fixed4 renderTex = tex2D(_MainTex, i.uv);
				return renderTex;
			}
			ENDCG
		}
	}
	FallBack "Diffuse"
}
```

Properties增加混合贴图和混合参数：


```cpp
_BlendTex ("Blend Texture", 2D) = "white" {}
		_Opacity ("Blend Opacity", Range(0, 1)) = 1
```

SubShader里声明同名变量：


```cpp
sampler2D _BlendTex;
			fixed _Opacity;
```

修改frag方法：


```cpp
fixed4 frag(v2f_img i) : COLOR
			{
				fixed4 renderTex = tex2D(_MainTex, i.uv);
				fixed4 blendTex = tex2D(_BlendTex, i.uv);

				fixed4 blended = getBlended(renderTex, blendTex);

				renderTex = lerp(renderTex, blended, _Opacity);

				return renderTex;
			}
```

getBlended方法：


```cpp
fixed4 getBlended()
			{
				fixed4 blended;
				blended = renderTex * blendTex;
				return blended;
			}
```

lerp是插值方法，计算from（第一个参数）和to（第二个参数）之间在p（第三个参数）上的插值，即lerp = from + (to - from) * p。



可以使用下面两张图片作为基础图片和混合图片：

![](https://img-blog.csdn.net/20161012134854549)


![](https://img-blog.csdn.net/20161012135034051)


获得效果：

![](https://img-blog.csdn.net/20161012135112505)





我们还可以尝试一下其他混合，例如把getBlended方法改成：



```cpp
fixed4 getBlended(fixed4 renderTex, fixed4 blendTex)
			{
				fixed4 blended;
				blended = renderTex + blendTex;
				return blended;
			}
```

效果：
![](https://img-blog.csdn.net/20161012135235079)





此外，还可以使用一种称为屏幕混合的方法：



```cpp
fixed4 getBlended(fixed4 renderTex, fixed4 blendTex)
			{
				fixed4 blended;
				blended = (1.0 - ((1.0 - renderTex) * (1.0 - blendTex)));
				return blended;
			}
```
效果：
![](https://img-blog.csdn.net/20161012135421658)





