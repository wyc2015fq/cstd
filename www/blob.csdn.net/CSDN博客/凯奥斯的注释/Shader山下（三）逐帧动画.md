# Shader山下（三）逐帧动画 - 凯奥斯的注释 - CSDN博客





2016年10月07日 23:07:35[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1337
所属专栏：[Shader山下](https://blog.csdn.net/column/details/12981.html)









# 逐帧动画是一种常见的动画形式（Frame By Frame），其原理是在“连续的关键帧”中分解动画动作，也就是在时间轴的每帧上逐帧绘制不同的内容，使其连续播放而成动画。 

例如下面这张图：

![](https://img-blog.csdn.net/20161007221616807)

这是一个精灵集，可以分解成八个动作。如果我们可以控制对象在一个时间间隔里只播放一个动作，那么看起来就是一个跑步的动画。

在实现帧动画之前，我们首先要知道UV坐标，UV坐标是表示场景对象上的某个点对应在贴图里的位置。

我们首先实现一个最简单的shader（参考[Shader山下（一）第一个Shader](http://blog.csdn.net/ecidevilin/article/details/52743971)）



```
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

为场景里的一个Plane添加使用这个shader的材质。


我们可以看到编辑器里显示：

![](https://img-blog.csdn.net/20161007223441118)


将上面的图片拖到Texture里面，并将Tiling的X和Offset的X都改为0.125，我们就可以看到：

![](https://img-blog.csdn.net/20161007223641323)


这便显示的是第二个动作，修改Offset的X为0.25便显示第三个动作，以此类推。

UV坐标的两个分量x,y都在[0,1]范围内，表示贴图横纵的标准化坐标，从(0,0)贴图的左下角，到(1,1)贴图的右上角，在这个范围外的会折叠到这个范围内（例如8.25就会转换成0.25）。


Tiling表示UV坐标的缩放倍数，计算UV的时候会乘以这个倍数，类似于UV=UV*Tiling。

Offset表示UV坐标的偏移量，在计算UV的时候会加上这个偏移量，类似于UV=UV+Offset。

最后的算法类似于UV=UV*Tiling+Offset。

当我们设置Tiling的X为0.125，就意味着最后所得出来的UV值都被限定在[0,0.125]之间，这样就只会显示一个动作。

将Offset的X设置为0.25，就意味着最后得出来的UV值被限定在[0.25,0.375]之间，也就是第三个动作。




那么怎么使用shader来实现帧动画呢？

修改上面的shader在Properties里添加：



```
_CellAmount("Cell Amount", float) = 1
		_Speed("Speed", Range(0.01, 32)) = 12
```

并在CGPROGRAM之后，ENDCG之前添加：




```
half _CellAmount;
		half _Speed;
```

_CellAmount表示动作的数量，_Speed表示播放的速度。


修改surf代码：



```
void surf (Input IN, inout SurfaceOutput o) {
			fixed2 texUV = IN.uv_MainTex;
			half cellUVX = 1 / _CellAmount;
			half timeVal = ceil(fmod(_Time.y * _Speed, _CellAmount));
			fixed uvx = texUV.x;
			uvx *= cellUVX;
			uvx += timeVal * cellUVX;
			texUV = fixed2(uvx, texUV.y);
			fixed4 c = tex2D (_MainTex, texUV);
			o.Albedo = c.rgb;
			o.Alpha = c.a;
		}
```




第一行，取出贴图的uv坐标。

第二行，计算一个动作的尺寸（UV坐标系下）。

第三行，根据时间和速度计算当前应该显示那个动作。

第四行，取出uv坐标的x分量。

第五行，乘以单个动作的尺寸（相当于Tiling）。

第六行，加上偏移量（相当于Offset）。

第七行，重新赋值uv坐标。

第八行：取出uv坐标的色彩值。

第九行：赋值色彩值。

第十行：赋值Aplha通道。




在编辑器里，复原Tiling(1，1)和Offset(0,0)，并把Cell Amount设置为8，然后点击播放，我们就可以看到一个奔跑的动画了。



