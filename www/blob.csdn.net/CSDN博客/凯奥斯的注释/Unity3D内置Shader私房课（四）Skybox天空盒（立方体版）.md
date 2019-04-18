# Unity3D内置Shader私房课（四）Skybox天空盒（立方体版） - 凯奥斯的注释 - CSDN博客





2016年11月20日 22:45:03[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：3347标签：[Unity3D																[Shader																[builtin																[Skybox																[HDR](https://so.csdn.net/so/search/s.do?q=HDR&t=blog)
个人分类：[Shader](https://blog.csdn.net/ecidevilin/article/category/6448925)

所属专栏：[Unity3D内置Shader私房课](https://blog.csdn.net/column/details/13117.html)](https://so.csdn.net/so/search/s.do?q=Skybox&t=blog)






# 我们除了可以使用Unity3D默认的天空盒之外，还可以自定义天空盒。可以使用Skybox/6Sided这个shader作为天空盒材质的着色器，它在内建Shader的

# DefaultResourcesExtra的目录中。

Unity内建Shader[下载地址](https://unity3d.com/get-unity/download/archive)。





首先我们创建一个Material，将Skybox/6Sided设置为着色器，我们看到需要设置6张贴图。

![](https://img-blog.csdn.net/20161120221331285)





然后我们在菜单栏里找到Window->Lighting，切换到Scene页，就可以看到修改Skybox材质的地方：

![](https://img-blog.csdn.net/20161120221709019)


修改为我们创建的材质，我们就可以看到：

![](https://img-blog.csdn.net/20161120222045489)


当我们旋转摄像机角度的时候，可以看到天空盒6个面，但是当我们调整摄像机坐标的时候，天空盒却不会发生变化。

![](https://img-blog.csdn.net/20161120222321767)














我们来看看这个shader里面都有什么：


```cpp
Shader "Skybox/6 Sided" {
Properties {
	_Tint ("Tint Color", Color) = (.5, .5, .5, .5)
	[Gamma] _Exposure ("Exposure", Range(0, 8)) = 1.0
	_Rotation ("Rotation", Range(0, 360)) = 0
	[NoScaleOffset] _FrontTex ("Front [+Z]   (HDR)", 2D) = "grey" {}
	[NoScaleOffset] _BackTex ("Back [-Z]   (HDR)", 2D) = "grey" {}
	[NoScaleOffset] _LeftTex ("Left [+X]   (HDR)", 2D) = "grey" {}
	[NoScaleOffset] _RightTex ("Right [-X]   (HDR)", 2D) = "grey" {}
	[NoScaleOffset] _UpTex ("Up [+Y]   (HDR)", 2D) = "grey" {}
	[NoScaleOffset] _DownTex ("Down [-Y]   (HDR)", 2D) = "grey" {}
}

SubShader {
	Tags { "Queue"="Background" "RenderType"="Background" "PreviewType"="Skybox" }
	Cull Off ZWrite Off
	
	CGINCLUDE
	#include "UnityCG.cginc"

	half4 _Tint;
	half _Exposure;
	float _Rotation;

	float4 RotateAroundYInDegrees (float4 vertex, float degrees)
	{
		float alpha = degrees * UNITY_PI / 180.0;
		float sina, cosa;
		sincos(alpha, sina, cosa);
		float2x2 m = float2x2(cosa, -sina, sina, cosa);
		return float4(mul(m, vertex.xz), vertex.yw).xzyw;
	}
	
	struct appdata_t {
		float4 vertex : POSITION;
		float2 texcoord : TEXCOORD0;
	};
	struct v2f {
		float4 vertex : SV_POSITION;
		float2 texcoord : TEXCOORD0;
	};
	v2f vert (appdata_t v)
	{
		v2f o;
		o.vertex = mul(UNITY_MATRIX_MVP, RotateAroundYInDegrees(v.vertex, _Rotation));
		o.texcoord = v.texcoord;
		return o;
	}
	half4 skybox_frag (v2f i, sampler2D smp, half4 smpDecode)
	{
		half4 tex = tex2D (smp, i.texcoord);
		half3 c = DecodeHDR (tex, smpDecode);
		c = c * _Tint.rgb * unity_ColorSpaceDouble.rgb;
		c *= _Exposure;
		return half4(c, 1);
	}
	ENDCG
	
	Pass {
		CGPROGRAM
		#pragma vertex vert
		#pragma fragment frag
		sampler2D _FrontTex;
		half4 _FrontTex_HDR;
		half4 frag (v2f i) : SV_Target { return skybox_frag(i,_FrontTex, _FrontTex_HDR); }
		ENDCG 
	}
	Pass{
		CGPROGRAM
		#pragma vertex vert
		#pragma fragment frag
		sampler2D _BackTex;
		half4 _BackTex_HDR;
		half4 frag (v2f i) : SV_Target { return skybox_frag(i,_BackTex, _BackTex_HDR); }
		ENDCG 
	}
	Pass{
		CGPROGRAM
		#pragma vertex vert
		#pragma fragment frag
		sampler2D _LeftTex;
		half4 _LeftTex_HDR;
		half4 frag (v2f i) : SV_Target { return skybox_frag(i,_LeftTex, _LeftTex_HDR); }
		ENDCG
	}
	Pass{
		CGPROGRAM
		#pragma vertex vert
		#pragma fragment frag
		sampler2D _RightTex;
		half4 _RightTex_HDR;
		half4 frag (v2f i) : SV_Target { return skybox_frag(i,_RightTex, _RightTex_HDR); }
		ENDCG
	}	
	Pass{
		CGPROGRAM
		#pragma vertex vert
		#pragma fragment frag
		sampler2D _UpTex;
		half4 _UpTex_HDR;
		half4 frag (v2f i) : SV_Target { return skybox_frag(i,_UpTex, _UpTex_HDR); }
		ENDCG
	}	
	Pass{
		CGPROGRAM
		#pragma vertex vert
		#pragma fragment frag
		sampler2D _DownTex;
		half4 _DownTex_HDR;
		half4 frag (v2f i) : SV_Target { return skybox_frag(i,_DownTex, _DownTex_HDR); }
		ENDCG
	}
}
}
```

需要注意的是：

1、在Properties里，为六张贴图添加了特性[NoScaleOffset]，这样就隐藏了Tiling和Offset。

2、Queue为Background，也就是最先渲染。

3、Cull Off关闭了（背面）剔除。

4、ZWrite Off关闭了深度写入。

5、总共六个Pass，分别用来渲染六张贴图。

6、六个Pass公用一个vert方法，调用了RotateAroundYInDegrees方法，会根据Rotation旋转天空盒。（sincos求Rotation的sin值和cos值）。

7、六个Pass里的frag方法都调用了skybox_frag方法，分别以贴图和贴图的HDR（参考百度百科[高动态光照渲染](http://baike.baidu.com/link?url=PB3kPRiby3fJ7gOYIZxP9IAiOYRdqvxJqAnYqc6fRdiqImQ2Vki7vNnmCwHkWoZuT2ute52iqgce8SqpWx3KEUgkkD4FvfWHeQnTICef1d9Rj9kt3WKSvAaZmepYviYs0P2XH9e3774siRKeUct_EFr0XfVpbM99z35oAUK_itWdaBAq84LKXw1Aw0W9DlVv47F7IQ0CbZdzbnMQdC6rpK)）向量。

8、unity_ColorSpaceDouble没有相关的文档，但是从Frame Debugger来看，是(2,2,2,2)。

9、DecodeHDR在CGIncludes/UnityCG.cginc里，代码：


```cpp
// Decodes HDR textures
// handles dLDR, RGBM formats
inline half3 DecodeHDR (half4 data, half4 decodeInstructions)
{
	// If Linear mode is not supported we can skip exponent part
	#if defined(UNITY_NO_LINEAR_COLORSPACE)
		return (decodeInstructions.x * data.a) * data.rgb;
	#else
		return (decodeInstructions.x * pow(data.a, decodeInstructions.y)) * data.rgb;
	#endif
}
```

另外，Skybox/Cubemap与Skybox/6Sided基本一致，只是将六张贴图换成了一个Cubemap，创建Cubemap的方法如图：

![](https://img-blog.csdn.net/20161120224053367)


恩，还是需要六张贴图。](https://so.csdn.net/so/search/s.do?q=builtin&t=blog)](https://so.csdn.net/so/search/s.do?q=Shader&t=blog)](https://so.csdn.net/so/search/s.do?q=Unity3D&t=blog)




