# Shader山下（二十三）GrabPass与UsePass - 凯奥斯的注释 - CSDN博客





2016年11月08日 21:45:57[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：3428
所属专栏：[Shader山下](https://blog.csdn.net/column/details/12981.html)









# 通道（Pass）是子着色器（SubShader）的组成部分，每个Pass代表对象被渲染一次。

例如：



```cpp
Shader "Custom/pass/Pass" {
	Properties {
	}
	SubShader {   
		Tags { "RenderType"="Opaque" }
		LOD 200
		Pass {
			CGPROGRAM
			#pragma vertex vert_img
			#pragma fragment frag
			#pragma fragmentoption ARB_precision_hint_fastest
			#include "UnityCG.cginc"

			fixed4 frag(v2f_img i) : COLOR
			{
				return fixed4(1,0,0,1);
			}
			ENDCG
        }
		Pass {

			Blend One One

			CGPROGRAM
			#pragma vertex vert_img
			#pragma fragment frag
			#pragma fragmentoption ARB_precision_hint_fastest
			#include "UnityCG.cginc"

			fixed4 frag(v2f_img i) : COLOR
			{
				return fixed4(0,1,0,1);
			}
			ENDCG
        }
	}
	FallBack "Diffuse"
}
```

这段Shader（的SubShader）中有两个Pass，第一个Pass的片元着色器返回了红色，第二个Pass的片元着色器返回了绿色，并且在第二个着色器中使用了Blend One One的混合方法（参考[Shader山下（十八）混合（Blend）命令](http://blog.csdn.net/ecidevilin/article/details/52864349)），这样两个颜色便会加在一起，得到如图所示的效果。


![](https://img-blog.csdn.net/20161108211903788)


如果要介绍多个Pass的写法，可能够写好多篇博客了，我们先放在一遍，以后再讲。

本文要介绍的是两种特殊的Pass：GrabPass和UsePass。




## GrabPass用于获取当前屏幕的纹理（Texture）。

例如：



```cpp
Shader "Custom/pass/Grab" {
	Properties {
	}
	SubShader {
		Tags { "RenderType"="Opaque" }
		LOD 200
		GrabPass
		{
		}   
		Pass {
			CGPROGRAM
			#pragma vertex vert_img
			#pragma fragment frag
			#pragma fragmentoption ARB_precision_hint_fastest
			#include "UnityCG.cginc"

            sampler2D _GrabTexture;

			fixed4 frag(v2f_img i) : COLOR
			{
				return tex2D(_GrabTexture, i.uv);
			}
			ENDCG
        }
	}
	FallBack "Diffuse"
}
```






我们可以得到这样的效果：

![](https://img-blog.csdn.net/20161108210424240)


GrabPass获取当前屏幕的纹理默认保存在_GrabTexture里。
但是并不会把顶点和片元信息传递给GPU，所以在GrabPass之后，我们需要再添加一个Pass，对_GrabTexture进行操作。

另外，我们可以在GrabPass里添加一个字符串来为修改纹理的变量名。

例如：



```cpp
GrabPass
		{
			"_GrabTex"
		}
```
那么原来的_GrabTexture就被取代，都要替换成_GrabTex。





GrabPass里面无法使用命令或者代码，除了Name（不知道意义何在）和Tags（参考[Shader山下（十九）标记Tag](http://blog.csdn.net/ecidevilin/article/details/52868742)）。






## UsePass实际上不是Pass，而是调用其他的Pass的指令。

例如我们为Custom/pass/Pass的第一个Pass命名。在Cg程序之外添加：



```cpp
Name "RED"
```

并且新建一个Shader：




```cpp
Shader "Custom/pass/Use" {
	Properties {
	}
	SubShader {
		Tags { "RenderType"="Opaque" }
		LOD 200
		UsePass "Custom/pass/Pass/RED"
	}
	FallBack "Diffuse"
}
```






我们就可以看到纯红色的对象了。

![](https://img-blog.csdn.net/20161108212719776)


UserPass后面跟的是Shader的名称/Pass的名称，Pass的名称必须全大写。（Pass里命名的时候没有这种要求，不过建议还是相互统一比较好）

我们上面说过，GrabPass里面可以使用Name命令，所以也可以使用UsePass调用某个GrabPass（同样不知道意义何在）。









