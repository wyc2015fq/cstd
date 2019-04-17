# Unity3D内置Shader私房课（一）视差Parallax - 凯奥斯的注释 - CSDN博客





2016年11月14日 22:37:52[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：3011
所属专栏：[Unity3D内置Shader私房课](https://blog.csdn.net/column/details/13117.html)









在Unity内建Shader的DefaultResourcesExtra的目录中，可以看到Normal-Parallax.shader这个文件。可以使用这个shader配合法线贴图创建视差效果来更好的表现对象表面的凹凸。

Unity内建Shader[下载地址](https://unity3d.com/get-unity/download/archive)。

首先我们在场景中添加一个Plane，然后新建一个Material，设置给Plane。修改Material的Shader为Legacy Shaders/Parallax Diffuse。

导入两张图片，一张法线贴图：

![](https://img-blog.csdn.net/20161114221201221)


一张高度贴图：

![](https://img-blog.csdn.net/20161114221220423)


其实高度贴图就是法线贴图的灰度图。导入高度贴图的时候要注意Alpha From Grayscale。




然后将这两张贴图分别设置为Material里的Normalmap和Heightmap(A)。

可以看到效果：

![](https://img-blog.csdn.net/20161114221431409)


调整Height值为0.05

![](https://img-blog.csdn.net/20161114221513723)


我们可以看到更加清晰的凹凸（假）。

调整Height值为0.08


![](https://img-blog.csdn.net/20161114221559285)


有些失真了。




Shader代码：



```cpp
Shader "Legacy Shaders/Parallax Diffuse" {
Properties {
	_Color ("Main Color", Color) = (1,1,1,1)
	_Parallax ("Height", Range (0.005, 0.08)) = 0.02
	_MainTex ("Base (RGB)", 2D) = "white" {}
	_BumpMap ("Normalmap", 2D) = "bump" {}
	_ParallaxMap ("Heightmap (A)", 2D) = "black" {}
}

CGINCLUDE
sampler2D _MainTex;
sampler2D _BumpMap;
sampler2D _ParallaxMap;
fixed4 _Color;
float _Parallax;

struct Input {
	float2 uv_MainTex;
	float2 uv_BumpMap;
	float3 viewDir;
};

void surf (Input IN, inout SurfaceOutput o) {
	half h = tex2D (_ParallaxMap, IN.uv_BumpMap).w;
	float2 offset = ParallaxOffset (h, _Parallax, IN.viewDir);
	IN.uv_MainTex += offset;
	IN.uv_BumpMap += offset;
	
	fixed4 c = tex2D(_MainTex, IN.uv_MainTex) * _Color;
	o.Albedo = c.rgb;
	o.Alpha = c.a;
	o.Normal = UnpackNormal(tex2D(_BumpMap, IN.uv_BumpMap));
}
ENDCG

SubShader {
	Tags { "RenderType"="Opaque" }
	LOD 500

	CGPROGRAM
	#pragma surface surf Lambert
	#pragma target 3.0
	ENDCG
}

SubShader {
	Tags { "RenderType"="Opaque" }
	LOD 500

	CGPROGRAM
	#pragma surface surf Lambert nodynlightmap
	ENDCG
}

FallBack "Legacy Shaders/Bumped Diffuse"
}
```

首先取出了高度图中的w值（也就是a值），然后使用ParallaxOffset方法计算uv的偏移量，并加到主贴图uv和法线贴图uv上，最后分别设置颜色和法线。


ParallaxOffset在CGIncludes目录的UnityCG.cginc文件中：




```cpp
// Calculates UV offset for parallax bump mapping
inline float2 ParallaxOffset( half h, half height, half3 viewDir )
{
	h = h * height - height/2.0;
	float3 v = normalize(viewDir);
	v.z += 0.42;
	return h * (v.xy / v.z);
}
```

通过设置的高度值和高度贴图中的值与视图向量进行运算，计算出uv偏移量。





内建shader中视差相关的shader有：


|Normal-Parallax|Legacy Shaders/Parallax Diffuse漫反射光照下的视差效果||
|----|----|----|
|Normal-ParallaxSpec|Legacy Shaders/Parallax Specular高光下的视差效果增加了高光颜色和高光值||
|Alpha-Parallax|Legacy Shaders/Transparent/Parallax DiffuseNormal-Parallax的半透明版本||
|Alpha-ParallaxSpec|Legacy Shaders/Transparent/Parallax SpecularNormal-ParallaxSpec的半透明版本||
|Illumin-Parallax|Legacy Shaders/Self-Illumin/Parallax DiffuseNormal-Parallax的自发光版本增加了自发光贴图和自发光值||
|Illumin-ParallaxSpec|Legacy Shaders/Self-Illumin/Parallax SpecularNormal-ParallaxSpec的自发光版本||
|Reflect-Parallax|Legacy Shaders/Reflective/Parallax DiffuseNormal-Parallax的反射版本增加了反射颜色和反射Cube||
|Reflect-ParallaxSpec|Legacy Shaders/Reflective/Parallax SpecularNormal-ParallaxSpec的反射版本||




