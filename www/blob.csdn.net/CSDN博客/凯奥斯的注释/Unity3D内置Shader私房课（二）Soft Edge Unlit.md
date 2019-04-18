# Unity3D内置Shader私房课（二）Soft Edge Unlit - 凯奥斯的注释 - CSDN博客





2016年11月17日 22:15:22[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1813
所属专栏：[Unity3D内置Shader私房课](https://blog.csdn.net/column/details/13117.html)









# 在Unity内建Shader的DefaultResourcesExtra的目录中，可以看到AlphaTest-SoftEdgeUnlit.shader这个文件。可以用来渲染草、树和叶子。

当绘制半透明的物体，例如草的时候，因为关闭了ZWrite，所以没有办法保证这些半透明物体正确的绘制顺序，就需要对它们进行深度排序。（参考[【Unity Shaders】Alpha Test和Alpha Blending](http://blog.csdn.net/candycat1992/article/details/41599167)）

而AlphaTest-SoftEdgeUnlit它将物体渲染分为了两个Pass，第一个Pass用来渲染不透明的部分，第二个pass用来渲染透明部分。

Unity内建Shader[下载地址](https://unity3d.com/get-unity/download/archive)。


我们先看看SubShader的指令：



```cpp
Tags { "Queue"="AlphaTest" "IgnoreProjector"="True" "RenderType"="TransparentCutout" }
	Lighting off
	
	// Render both front and back facing polygons.
	Cull Off
```

Tags（参考[Shader山下（十九）标记Tag](http://blog.csdn.net/ecidevilin/article/details/52868742)）


它的渲染序列是AlphaTest（2450），忽略投影，渲染类型为透明裁切。

Lighing off

不受光照影响

Cull Off

剔除关闭




接着是第一个Pass的frag方法：



```cpp
fixed4 frag (v2f i) : SV_Target
			{
				half4 col = _Color * tex2D(_MainTex, i.texcoord);
				clip(col.a - _Cutoff);
				UNITY_APPLY_FOG(i.fogCoord, col);
				return col;
			}
```
使用clip方法检测Alpha是否小于_Cutoff，如果小于便会舍弃掉该片元。


UNITY_APPLY_FOG将雾效（参考[Unity3D开发小贴士（十）雾效](http://blog.csdn.net/ecidevilin/article/details/53178562)）颜色与当前颜色根据雾效参数进行插值。





然后我们看第二个Pass的指令：



```cpp
Tags { "RequireOption" = "SoftVegetation" }
		
		// Dont write to the depth buffer
		ZWrite off
		
		// Set up alpha blending
		Blend SrcAlpha OneMinusSrcAlpha
```

Tags


需要选项SoftVegetation（默认是开启的，但是没有找到在哪里设置，求指教）
ZWrite off

不将深度值写入ZBuffer（深度缓存，用来判断对象前后顺序，以及片元是否会被剔除掉）

Blend SrcAlpha OneMinusSrcAlpha

使用常规混合方式（参考[Shader山下（十八）混合（Blend）命令](http://blog.csdn.net/ecidevilin/article/details/52864349)）




第二个Pass的frag方法：



```cpp
fixed4 frag (v2f i) : SV_Target
			{
				half4 col = _Color * tex2D(_MainTex, i.texcoord);
				clip(-(col.a - _Cutoff));
				UNITY_APPLY_FOG(i.fogCoord, col);
				return col;
			}
```
舍弃掉Alpha大于_Cutoff的片元。





最后我们看一看效果，使用官方Environment Package里面的Conifer_Needles_Desktop作为贴图：

Base Alpha cutoff = 0.01（接近标准的Alpha Test效果）

![](https://img-blog.csdn.net/20161117221259612)


BaseAlphacutoff = 0.5


![](https://img-blog.csdn.net/20161117221306612)


BaseAlphacutoff = 0.9


![](https://img-blog.csdn.net/20161117221312153)




