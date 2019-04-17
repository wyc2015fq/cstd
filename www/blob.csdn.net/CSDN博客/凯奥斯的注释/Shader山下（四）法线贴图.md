# Shader山下（四）法线贴图 - 凯奥斯的注释 - CSDN博客





2016年10月08日 09:01:25[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1134
所属专栏：[Shader山下](https://blog.csdn.net/column/details/12981.html)









# 法线向量表示某个点上垂直于该点的向量，而法线贴图（Normal Map）就是保存这些法线的贴图。实际应用中，经常使用法线贴图为低模添加细节。

![](https://img-blog.csdn.net/20161008084019987)


上图来源于百度百科[法线贴图](http://baike.baidu.com/link?url=7UgvkYSXAtYyIoBSLDwuCE4YP2uiph_cUbjDk4258bRILxDZl_QGEliiJ5PaeEHy41QJJBlcZeQPYBouOv5PApeDp9SZJ4d-rFX8qJ124wGCO9RY2PWsEDAu0FJgs7nv)。




本文就介绍一下，怎样使用shader来添加法线贴图。

首先依旧是最简单的漫反射shader：



```
Shader "Custom/NormalMap" {
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
			fixed2 texUV = IN.uv_MainTex;
			fixed4 c = tex2D (_MainTex, texUV);
			o.Albedo = c.rgb;
			o.Alpha = c.a;
		}

		ENDCG
	}
	FallBack "Diffuse"
}
```
在Properties里添加：



`_NormalMap ("Normal Map", 2D) = "white" {}`
并在SubShader的CGPROGRAM之后，surf方法之前添加：



`sampler2D _NormalMap;`
这样我们就可以在编辑器里看到多了一个Normal Map的选项了。


在Input结构里添加：


`half2 uv_NormalMap;`
这样我们就可以在surf方法里调用法线贴图的纹理坐标了。


修改surf方法：



```
void surf (Input IN, inout SurfaceOutput o) {
			fixed4 c = tex2D (_MainTex, IN.uv_MainTex);
			o.Albedo = c.rgb;
			o.Alpha = c.a;
			fixed3 n = UnpackNormal(tex2D (_NormalMap, IN.uv_NormalMap));
			o.Normal = n;
		}
```

导入下面这张图片：


![](https://img-blog.csdn.net/20161008085430790)


或者自己找一个也没关系，记得Texture Type设置为Normal Map。（注：因为法线一般都偏向z轴方向，所以法线贴图一般偏蓝色）

为使用本文shader的材质设置Normal Map，并把它应用在场景对象上，我们就可以看到：

![](https://img-blog.csdn.net/20161008085656071)


添加主贴图后的效果：

![](https://img-blog.csdn.net/20161008085754088)




