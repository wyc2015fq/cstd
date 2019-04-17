# Shader山下（二）光照模型——漫反射 - 凯奥斯的注释 - CSDN博客





2016年10月07日 14:46:09[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1193
所属专栏：[Shader山下](https://blog.csdn.net/column/details/12981.html)









# 光照模型在shader中是一个非常重要的部分，主要用于对象表面某个光照位置的颜色计算。

在讲光照模型之前，我们先回顾一下上篇[Shader山下（一）第一个Shader](http://blog.csdn.net/ecidevilin/article/details/52743971)的代码：



```
Shader "Custom/BasicDiffuse" {
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
其中这一句：
`#pragma surface surf Lambert`
代表这个shader是一个着色器，着色方法名称为surf，光照模型使用的是内置的Lambert。


我们修改这一句，改为：


`#pragma surface surf BasicDiffuse`
这里我们使用了一个自定义的光照模型，名为BasicDiffuse。具体的方法名为LightingBasicDiffuse：




```
inline fixed4 LightingBasicDiffuse(SurfaceOutput s, fixed3 lightDir, fixed atten)
		{
			fixed difLight = max(0, dot (s.Normal, lightDir));
			fixed4 col;
			col.rgb = s.Albedo * _LightColor0.rgb * (difLight * atten * 2);
			col.a = s.Alpha;
			return col;
		}
```




如果你把这个shader用在场景中的物体上，你不会发现与之前的shader有任何变化。因为这其实就是Lambert模型。

第一个参数s与surf里的参数类型一致，实际上就是在surf方法处理完之后，传递给LightingBasicDiffuse的。

第二个参数lightDir表示光照向量。

第三个参数atten表示光衰减系数。

第一行将像素的法线向量与光照向量进行点积操作。操作返回[-1,1]的值，表示两个向量之间的夹角关系，1表示二者同向平行，-1表示二者反向平行，0表示二者垂直。然后如果点积值值小于0，则取0，赋值给difLight。我们知道，当二者平行的时候，也就意味着光照向量与该像素垂直，也就是该点可以接收到最多的光照。

第二行声明了一个颜色变量col。

第三行用s的色彩值与光照颜色_LightColor0（从Unity场景中获取的）相乘（rgb三个分量分别相乘），最后再乘以difLight和atten并乘以2（[参考](https://forum.unity3d.com/threads/why-atten-2.94711/)）。

第四行将s的Alpha值赋值给color的a。

第五行返回col。

就是根据像素点的法线方向与光照方向将光照颜色与像素颜色进行混合。




理解这个最简单的光照模型之后，我们可以考虑自己修改。例如，我们在Properties里面添加：


`		_LightRate ("Light Rate", Range(0,1)) = 0.5`并在SubShader的CGPROGRAM后面ENDCG之前添加：



`fixed _LightRate;`
最后修改光照模型：




```
inline fixed4 LightingBasicDiffuse(SurfaceOutput s, fixed3 lightDir, fixed atten)
		{
			fixed difLight = max(0, dot (s.Normal, lightDir));
			difLight = difLight * (1 - _LightRate) + _LightRate;
			fixed4 col;
			col.rgb = s.Albedo * _LightColor0.rgb * (difLight * atten * 2);
			col.a = s.Alpha;
			return col;
		}
```

当我们在编辑器里调节Light Rate的值的时候，我们会发现，模型的颜色也会相应的发生变化。

如果Light Rate为0，那么就跟之前的Lambert没有什么区别。如果为0.5就是著名的Half Lambert，这是一种用于在低光照区域照亮物体的技术。它基本上提高了材质和物体表面周围的漫反射光照。





接着我们还可以实现一些更有意思的光照模型。

在Properties里面添加：



```
_StartColor("Start Color", Color) = (1,0,0,1)
		_EndColor("End Color", Color) = (0,1,1,1)
```
在SubShader的CGPROGRAM后面ENDCG之前添加：




```
fixed4 _StartColor;
		fixed4 _EndColor;
```

最后修改光照模型：




```
inline fixed4 LightingBasicDiffuse(SurfaceOutput s, fixed3 lightDir, fixed atten)
		{
			fixed difLight = max(0, dot (s.Normal, lightDir));
			difLight = difLight * (1 - _LightRate) + _LightRate;
			fixed3 ramp = lerp(_StartColor, _EndColor, difLight).rgb;
			fixed4 col;
			col.rgb = s.Albedo * _LightColor0.rgb * ramp;
			col.a = s.Alpha;
			return col;
		}
```

如果编译通过，你可能会在场景中得到这样一个Cube：


![](https://img-blog.csdn.net/20161007142926366)
调整Light Rate和颜色，你还可能得到这样一个Cube：

![](https://img-blog.csdn.net/20161007143013257)


如果换成Sphere，你会发现，这其实是渐变色：

![](https://img-blog.csdn.net/20161007143314837)







