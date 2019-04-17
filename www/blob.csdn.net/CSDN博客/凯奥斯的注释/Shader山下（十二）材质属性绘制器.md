# Shader山下（十二）材质属性绘制器 - 凯奥斯的注释 - CSDN博客





2016年10月12日 22:11:30[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1518
所属专栏：[Shader山下](https://blog.csdn.net/column/details/12981.html)









# Unity5增加了一个叫MaterialPropertyDrawer的东西，可以使用它们标记在shader的Properties里面，扩充shader的一些参数在编辑器上的样式。

本文以[Shader山下（十一）纹理混合](http://blog.csdn.net/ecidevilin/article/details/52796350)中的shader为例子，来介绍一下这些标记。



```cpp
Shader "Custom/Frag/Blend" {
	Properties {
		_MainTex ("Albedo (RGB)", 2D) = "white" {}
		_BlendTex ("Blend Texture", 2D) = "white" {}
		_Opacity ("Blend Opacity", Range(0, 1)) = 1
		[Space(50)][Header(MaterialPropertyDrawer)]
		[KeywordEnum(None, Add, Multiply, Screen)] _BlendMode ("Blend Mode", Float) = 0
//		[Enum(None, 0, Add, 1, Multiply, 2, Screen ,3)] _BlendMode ("Blend Mode", Float) = 0
		[Space]
		[Toggle] _Invert ("Invert?", Float) = 0
		[Toggle(ENABLE_DOUBLE)] _Double ("Double?", Float) = 0
		[PowerSlider(3.0)] _Power ("Power", Range(1,1024)) = 1
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
			sampler2D _BlendTex;
			fixed _Opacity;
			int _BlendMode;
			bool _Invert;
			bool _Double;
			fixed4 getBlended(fixed4 renderTex, fixed4 blendTex)
			{
				fixed4 blended;

				switch (_BlendMode)
				{
					case 0:
					blended = renderTex;
					break;
					case 1:
					blended = renderTex + blendTex;
					break;
					case 2:
					blended = renderTex * blendTex;
					break;
					case 3:
					blended = (1.0 - ((1.0 - renderTex) * (1.0 - blendTex)));
					break;
				}


				if (_Invert)
				{
					blended = 1 - blended;
				}

				if (_Double)
				{
					blended *= 2;
				}

				return blended;
			}

			fixed4 frag(v2f_img i) : COLOR
			{
				fixed4 renderTex = tex2D(_MainTex, i.uv);
				fixed4 blendTex = tex2D(_BlendTex, i.uv);

				fixed4 blended = getBlended(renderTex, blendTex);

				renderTex = lerp(renderTex, blended, _Opacity);

				return renderTex;
			}
			ENDCG
		}
	}
	FallBack "Diffuse"
}
```


我们的材质编辑器就变成了这样：
![](https://img-blog.csdn.net/20161012214134687)





首先是space标记，可以带参数也可以不带。不带参数时，默认产生如图中Blend Mode与Invert？之间的距离。

然后是Header标记，产生了一行加粗的文本，要注意参数不能加引号，包括后面的一些标记（一个奇怪的设定）。

KeywordEnum是产生一个下拉框


![](https://img-blog.csdn.net/20161012214548376)


对应数值从0开始保存到_BlendMode里面。修改这些值，就会出现[Shader山下（十一）纹理混合](http://blog.csdn.net/ecidevilin/article/details/52796350)中各种不同的混合效果。

并且在这里，它跟被注释掉的哪一行Enum是等同的。而Enum与之区别在于，设置的数值可以不是连续并有序的。

Enum最多支持7对值，而KeywordEnum最多支持9个值。

Toggle会创建一个开关，把bool值保存到_Invert和_Double里面，至于带参数和不带参数之间的区别，我们稍后会讲到。

最后一个PowerSlider只做展示用，它创建了一个指数增长的滑动条，我们可以把值改为512：

![](https://img-blog.csdn.net/20161012215138529)


你会发现本来应该在中间位置的滑动按钮跑到了右边。




其实还有另外一种形式可以使用，增加下面这些代码：



```cpp
#pragma shader_feature _INVERT_ON
        	#pragma shader_feature ENABLE_DOUBLE
			#pragma multi_compile _BLENDMODE_NONE _BLENDMODE_ADD _BLENDMODE_MULTIPLY _BLENDMODE_SCREEN
			fixed4 getBlendedMacro(fixed4 renderTex, fixed4 blendTex)
			{
				fixed4 blended;
				#if _BLENDMODE_NONE
					blended = renderTex;
				#elif _BLENDMODE_ADD
					blended = renderTex + blendTex;
				#elif _BLENDMODE_MULTIPLY
					blended = renderTex * blendTex;
				#elif _BLENDMODE_SCREEN
					blended = (1.0 - ((1.0 - renderTex) * (1.0 - blendTex)));
				#endif
				#if _INVERT_ON
					blended = 1 - blended;
				#endif

				#if ENABLE_DOUBLE
					blended *= 2;
				#endif
				return blended;
			}
```




并把frag方法里面获取混合色彩值的方法改为：



```cpp
fixed4 blended = getBlendedMacro(renderTex, blendTex);
```


结果是等效的。
其中_INVERT_ON对应于[Toggle] _Invert，命名规则是变量名的大写形式+_ON。

ENABLE_DOUBLE对应于[Toggle(ENABLE_DOUBLE)] _Double
 ，ENABLE_DOUBLE替换了_DOUBLE_ON（再声明这个宏也是没用的）。


_BLENDMODE_NONE _BLENDMODE_ADD _BLENDMODE_MULTIPLY _BLENDMODE_SCREEN对应于[KeywordEnum(None, Add, Multiply, Screen)] _BlendMode，命名规则为变量名的大写形式+枚举名的大写形式。


而需要注意的是，Enum标记无法使用这些宏。

最后看看材质效果（貌似并没有什么意义）

![](https://img-blog.csdn.net/20161012220639089)


（Blend Model=Screen,Invert?=On,Double?=On,Power=whatever）



