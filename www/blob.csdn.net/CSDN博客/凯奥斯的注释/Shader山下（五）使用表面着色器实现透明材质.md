# Shader山下（五）使用表面着色器实现透明材质 - 凯奥斯的注释 - CSDN博客





2016年10月08日 21:30:41[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：3312
所属专栏：[Shader山下](https://blog.csdn.net/column/details/12981.html)









# Unity中的表面着色器(Surface Shaders)是一种代码生成的方法，它比底层的顶点(vertex)/像素(pixel)着色程序更容易编写出受光照的着色器（引用自[Unity圣典](http://www.ceeger.com/Components/SL-SurfaceShaders.html)，顺便吐槽一下渣翻译，或者直接看[原版](https://docs.unity3d.com/Manual/SL-SurfaceShaders.html)）。

前几篇中，我们使用的就是表面着色器，参考[Shader山下（一）第一个Shader](http://blog.csdn.net/ecidevilin/article/details/52743971)。不过它们都是不透明的着色器，那么怎样编写一个透明的着色器呢？方法很简单，这里简单介绍一下。

首先我们依旧还是需要一个最简单的漫反射表面着色器：



```
Shader "Custom/TransparentSurface" {
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

然后在Tags里面增加：



`"Queue"="Transparent"`



注意两个Tag中间可以用空格或者回车隔开，或者不隔开，但是不要用分号（其他符号也不行）。

这句话表明渲染顺序在3000（Transparent对应的值）。使用这个Tag可以保证shader在所有不透明物体之后被渲染。

然后将pragma这句修改成：


`#pragma surface surf Lambert alpha`关于可选参数，可以参考上面的给过链接的[Unity圣典](http://www.ceeger.com/Components/SL-SurfaceShaders.html)。

这样就可以了，我们可以选取一张png图片作为贴图，例如下面这一张：


![](https://img-blog.csdn.net/20161008210723444)


就可以得到这样的结果，上面的是使用我们编写的Shader，下面的是使用Unity内置的着色器Standard。

![](https://img-blog.csdn.net/20161008210844866)


然后我们可以为贴图添加一个颜色，Properties里面：


`_MainTint("Color", Color)= (1,1,1,1)`
SubShader里面：



`fixed4 _MainTint;`
修改surf方法：




```
void surf (Input IN, inout SurfaceOutput o)   
		{  
			fixed4 c = tex2D(_MainTex, IN.uv_MainTex) * _MainTint;
			o.Albedo = c.rgb;
			o.Alpha = c.a;
		}
```

最后在编辑器里调整颜色为0xFFFFFF80，就可以得到半透明效果：


![](https://img-blog.csdn.net/20161008211141117)


学过图形学的话，应该知道这是Blend方法~~*（如果像我一样没学过的也没关系，反正早晚都会去学的……）*~~，是将已经绘制好的像素与当前像素的颜色进行混合得到一个半透明的效果。

我们也可以使用AlphaTest，这是一种非实体即透明的方法，即像素点满足一定条件时会完全显示出来（不透明），不满足时就会被舍弃掉。

需要把pragma这一句改成：


`#pragma surface surf Lambert alphatest:_Cut`这样就表示使用了AlphaTest方法，并声明了_Cut参数，Alpha大于_Cut这个值的像素点才会被显示出来。


因为这里已经声明了SubShader里的_Cut变量，所以只需要在Properties里面添加：


`_Cut("Cut", Range(0,1)) = 0.5`其他不变，我们看效果：


![](https://img-blog.csdn.net/20161008212302233)


如果把颜色改为0xFFFFFF79就会变成：

![](https://img-blog.csdn.net/20161008212540243)




