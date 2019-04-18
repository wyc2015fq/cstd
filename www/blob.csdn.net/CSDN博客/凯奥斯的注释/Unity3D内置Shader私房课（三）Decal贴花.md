# Unity3D内置Shader私房课（三）Decal贴花 - 凯奥斯的注释 - CSDN博客





2016年11月18日 22:53:54[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：5822
所属专栏：[Unity3D内置Shader私房课](https://blog.csdn.net/column/details/13117.html)









# 在Unity内建Shader的DefaultResourcesExtra的目录中，有一个很简单却很实用的shader——Decal。这是一个贴花着色器，可以在模型的表面添加一个贴花纹理。

Unity内建Shader[下载地址](https://unity3d.com/get-unity/download/archive)。





如图所示：

![](https://img-blog.csdn.net/20161118224330901)


（程序员的审美也就这样吧……）




我们看看Decal的代码：


```cpp
Shader "Legacy Shaders/Decal" {
Properties {
	_Color ("Main Color", Color) = (1,1,1,1)
	_MainTex ("Base (RGB)", 2D) = "white" {}
	_DecalTex ("Decal (RGBA)", 2D) = "black" {}
}

SubShader {
	Tags { "RenderType"="Opaque" }
	LOD 250
	
CGPROGRAM
#pragma surface surf Lambert

sampler2D _MainTex;
sampler2D _DecalTex;
fixed4 _Color;

struct Input {
	float2 uv_MainTex;
	float2 uv_DecalTex;
};

void surf (Input IN, inout SurfaceOutput o) {
	fixed4 c = tex2D(_MainTex, IN.uv_MainTex);
	half4 decal = tex2D(_DecalTex, IN.uv_DecalTex);
	c.rgb = lerp (c.rgb, decal.rgb, decal.a);
	c *= _Color;
	o.Albedo = c.rgb;
	o.Alpha = c.a;
}
ENDCG
}

Fallback "Legacy Shaders/Diffuse"
}
```

这是一个表面着色器，代码非常简单。




唯一要注意的是：

lerp (c.rgb, decal.rgb, decal.a)

实际上就等于：

decal.a*(decal.rgb-c.rgb)+c.rgb

也就等于：

decal.a*decal+(1-decal.a)*c.rgb




也就相当于使用了常规的混合模式（参考[Shader山下（十八）混合（Blend）命令](http://blog.csdn.net/ecidevilin/article/details/52864349)）

Blend SrcAlpha OneMinusSrcAlpha

（因为是Opaque，所以透明度也就无所谓了）






