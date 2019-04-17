# Shader山下（六）镜面高光 - 凯奥斯的注释 - CSDN博客





2016年10月09日 09:01:56[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：2706
所属专栏：[Shader山下](https://blog.csdn.net/column/details/12981.html)









# [Shader山下（二）光照模型](http://blog.csdn.net/ecidevilin/article/details/52749173)中介绍了漫反射的光照模型，漫反射是投射在粗糙表面上的光向各个方向反射的现象。与之相对的是镜面反射，也就是当平行入射的光线射到这个反射面时，仍会平行地向一个方向反射出来的现象。

二者区别如图：

![](https://img-blog.csdn.net/20161009074548585)


Unity里内置的漫反射光照模型是Lambert，而镜面高光模型则是BlinnPhong。

首先我们还是需要一个最简单的漫反射着色器：



```
Shader "Custom/SpecularTest" {
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

修改pragma这一句：



`#pragma surface surf BlinnPhong`

这样就是用了BlinnPhong的高光模型，但是应用到场景里时，并没有什么变化，因为参数还没有设置。


Properties里添加：



```
_SpecColor ("Specular Color", Color) = (1,1,1,1)
		_SpecPower ("Specular Power", Range(0,1)) = 0.05
		_SpecGloss ("Specular Gloss", Range(0,2)) = 1
```

因为Unity3D已经声明过了_SpecColor，所以只需要在SubShader里声明：




```
fixed _SpecPower;
		fixed _SpecGloss;
```

修改surf方法：




```
void surf (Input IN, inout SurfaceOutput o) {
			fixed2 texUV = IN.uv_MainTex;
			fixed4 c = tex2D (_MainTex, texUV);
			o.Albedo = c.rgb;
			o.Alpha = c.a;
			o.Specular = _SpecPower;
			o.Gloss = _SpecGloss;
		}
```

使用Unity自带的图片Default-Particle作为贴图，并调整Specular Color为0x0000FFFF，我们可以得到这样的结果：


![](https://img-blog.csdn.net/20161009080248546)


如果你下载了Unity的内建shader（[下载地址](https://unity3d.com/get-unity/download/archive)），那么可以打开CGIncludes/Lighting.cginc查看LightingBlinnPhong的源码：



```
inline fixed4 UnityBlinnPhongLight (SurfaceOutput s, half3 viewDir, UnityLight light)
{
	half3 h = normalize (light.dir + viewDir);
	
	fixed diff = max (0, dot (s.Normal, light.dir));
	
	float nh = max (0, dot (s.Normal, h));
	float spec = pow (nh, s.Specular*128.0) * s.Gloss;
	
	fixed4 c;
	c.rgb = s.Albedo * light.color * diff + light.color * _SpecColor.rgb * spec;
	c.a = s.Alpha;

	return c;
}

inline fixed4 LightingBlinnPhong (SurfaceOutput s, half3 viewDir, UnityGI gi)
{
	fixed4 c;
	c = UnityBlinnPhongLight (s, viewDir, gi.light);

	#if defined(DIRLIGHTMAP_SEPARATE)
		#ifdef LIGHTMAP_ON
			c += UnityBlinnPhongLight (s, viewDir, gi.light2);
		#endif
		#ifdef DYNAMICLIGHTMAP_ON
			c += UnityBlinnPhongLight (s, viewDir, gi.light3);
		#endif
	#endif

	#ifdef UNITY_LIGHT_FUNCTION_APPLY_INDIRECT
		c.rgb += s.Albedo * gi.indirect.diffuse;
	#endif

	return c;
}
```

虽然与shader中的写法不同，但是基本可以看出它的算法。


我们按照它在shader里面添加一个CustomBlinnPhong的光照模型。

先修改pragma这一句：


`#pragma surface surf CustomBlinnPhong`
具体实现：




```
inline fixed4 LightingCustomBlinnPhong(SurfaceOutput s, fixed3 lightDir, fixed3 viewDir, fixed atten)
		{
			half3 h = normalize (lightDir + viewDir);

			fixed diff = max (0, dot (s.Normal, lightDir));

			
			float nh = max (0, dot (s.Normal, h));

			float spec = pow (nh, s.Specular*128.0) * s.Gloss;

			fixed4 col;
			col.rgb = s.Albedo * _LightColor0.rgb * diff + _LightColor0.rgb * _SpecColor.rgb * spec;
			col.a = s.Alpha;
			col *= atten;

			return col;
		}
```




修改之后，场景里面物体并没有什么改变，证明我们写~~*（蒙）*~~对了。

简单分析一下：

第一行将光照向量与实现向量相加并标准化，得到的结果成为半角向量，这是通过视线方向和入射光线方向的角平分线向量来基础地模拟反射向量
 。

第二行将像素法线向量与光照向量点积，获得光照值diff。

第三行将像素法线向量与半角向量点积，获得高光值nh。

第四行将nh做幂运算，幂为s.Specular*128.0（这也就是为什么我们在编辑器里需要把Specular Power调的很小才能看到Cube上的高光），并乘以s.Gloss。得出来的spec表示高光强度。

第五行声明一个col。

第六行计算色彩值，_LightColor0是Unity中的默认光照颜色。+号之前将光照与像素色彩值混合得到一个基础色彩值，+号之后将光照颜色与高光颜色混合得到高光色彩值。

第七行赋值透明度。

第八行将col于atten（光衰减系数）相乘，在CGIncludes/Lighting.cginc没有体现，但其实在预处理的时候就将光照颜色乘上了这个数值。

第九行返回col。




接着我们可以实现一个高光遮罩的效果。

首先在Properties里添加遮罩贴图：


`_SpecTex ("Specular Mask", 2D) = "white" {}`SubShader里声明同名变量：



`sampler2D _SpecTex;`
Input结构里面需要传入遮罩贴图的uv坐标：




```
struct Input {
			float2 uv_MainTex;
			float2 uv_SpecTex;
		};
```

另外我们需要声明一个自定义的SurfaceOutput来传递遮罩贴图的色彩值：




```
struct CustomSurfaceOutput {
		    half3 Albedo;
		    half3 Normal;
		    half3 Emission;
		    half Specular;
		    half Gloss;
		    half Alpha;
		    half3 SpecularColor;
		};
```

增加了类型为half3的SpecularColor。


修改surf，注意输出类型变了：



```
void surf (Input IN, inout CustomSurfaceOutput o) {
			fixed4 c = tex2D (_MainTex, IN.uv_MainTex);
			fixed4 m = tex2D (_SpecTex, IN.uv_SpecTex) * _SpecColor;
			o.Albedo = c.rgb;
			o.Alpha = c.a;
			o.Specular = _SpecPower;
			o.Gloss = _SpecGloss;
			o.SpecularColor = m.rgb;
		}
```


其他不变，只是第二行将遮罩贴图上的像素颜色与_SpecColor相乘得到m，并且最后一行将m的rgb分量赋值给o.SpecularColor。


接着修改LightingCustomBlinnPhong，改动比较少，首先方法声明：


`inline fixed4 LightingCustomBlinnPhong(CustomSurfaceOutput s, fixed3 lightDir, fixed3 viewDir, fixed atten)`注意输入类型变了。


接着把_SpecColor.rgb替换为s.SpecularColor就OK了。也就是修改第六行：


`col.rgb = s.Albedo * _LightColor0.rgb * diff + _LightColor0.rgb * s.SpecularColor * spec;`




场景中，使用Unity自带的Knob图片作为高光遮罩，我们可以得到：

![](https://img-blog.csdn.net/20161009085730338)




