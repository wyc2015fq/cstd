# Shader山下（十四）顶点片段着色器处理光照 - 凯奥斯的注释 - CSDN博客





2016年10月14日 09:06:22[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：2342
所属专栏：[Shader山下](https://blog.csdn.net/column/details/12981.html)









# [Shader山下（二）光照模型——漫反射](http://blog.csdn.net/ecidevilin/article/details/52749173)里介绍了如何为表面着色器添加光照模型，但是在顶点片段（vertex-fragment）着色器里如何处理光照呢？

本文就实现一个与内置Lambert光照模型相同效果的VF着色器。



```cpp
Shader "Custom/Frag/Light" {
	Properties {
		_MainTex ("Base (RGB)", 2D) = "white" {}
	}
	SubShader {
		Tags { "RenderType"="Opaque" }
		LOD 200
		
		Pass {
			Tags { "LightMode" = "ForwardBase" }
			
			CGPROGRAM
			
			#pragma multi_compile_fwdbase
			
			#pragma vertex vert
			#pragma fragment frag
			
			#include "UnityCG.cginc"
			#include "Lighting.cginc"
			#include "AutoLight.cginc"
			
			sampler2D _MainTex;
			
			float4 _MainTex_ST;
			
			struct a2v {
				float4 vertex : POSITION;
                fixed3 normal : NORMAL;
                fixed4 texcoord : TEXCOORD0;
				fixed4 color : COLOR;
			};
			
			struct v2f {
				float4 pos : POSITION;
				float2 uv : TEXCOORD0;
				float3 worldNormal : TEXCOORD1;
				float3 lightDir : TEXCOORD2;
				float3 viewDir : TEXCOORD3;
				LIGHTING_COORDS(4,5)				//1
			};

			v2f vert(a2v v) {
				v2f o;
				
				//Transform the vertex to projection space
				o.pos = mul(UNITY_MATRIX_MVP, v.vertex);
				//Get the UV coordinates
				o.uv = TRANSFORM_TEX (v.texcoord, _MainTex); 
				//Normal
				o.worldNormal = mul(SCALED_NORMAL, (float3x3)_World2Object);			//2
				//Light Direction
				o.lightDir = mul((float3x3)_Object2World, ObjSpaceLightDir(v.vertex));	//3
				//View Direction
				o.viewDir = mul((float3x3)_Object2World, ObjSpaceViewDir(v.vertex));	//4

				//Shadow
  				TRANSFER_VERTEX_TO_FRAGMENT(o);											//5

				

				return o;
			}

			inline fixed4 LightingFragLambert(fixed4 fcol, fixed3 lightDir, fixed atten, half3 worldNormal)
			{
				fixed difLight = max(0, dot (normalize(worldNormal), normalize(lightDir)));
				fixed4 col;
				col.rgb = fcol.rgb * _LightColor0.rgb * (difLight * atten);
				col.a = fcol.a;

				return col;
			}

			half3 calDiffuse(half3 pos, half3 worldNormal)
			{
				half3 ambient = Shade4PointLights (
						unity_4LightPosX0, unity_4LightPosY0, unity_4LightPosZ0,
						unity_LightColor[0].rgb, unity_LightColor[1].rgb, unity_LightColor[2].rgb, unity_LightColor[3].rgb,
						unity_4LightAtten0, pos, worldNormal);
				ambient = ShadeSHPerVertex (worldNormal, ambient);
				return ShadeSHPerPixel (worldNormal, ambient);
			}

			float4 frag(v2f i) : COLOR {
				fixed4 texColor = tex2D(_MainTex, i.uv);	

				fixed4 fragColor = LightingFragLambert(texColor, i.lightDir, LIGHT_ATTENUATION(i), i.worldNormal);//6
				half3 diffuse = calDiffuse(i.pos, i.worldNormal);							//7
				fragColor.rgb += texColor.rgb * diffuse;//8

				return fragColor;
			}
			ENDCG
		}
	} 
	FallBack "Diffuse"
```





比较关键或者需要解释的几个点我都用注释加了序号。


第1处，这个宏对应了第5处，LIGHTING_COORDS声明了两个变量，一个是_LightCoord，一个是_ShadowCoord，而第5处计算了这两个值（用兴趣的可以研究一下AutoLight.cginc，这里不多赘述）。在第6处的LIGHT_ATTENUATION（也在AutoLight.cginc里面）计算光衰减值（也就是我们熟悉的atten）时使用了这两个值。

第2处，计算法线向量。

第3处，计算光照向量。

第4处，计算视图向量。

第6处，为了对应表面着色器，我们特意定义了一个LightingFragLambert方法。简单讲一下，就是根据像素点与光照方向的角度，将像素色彩值与光照色彩值相乘。

第7处，这里定义了一个vertDiffuse的方法，用于计算顶点漫反射的色彩值，它在第8处使用到。

第8处，第7处得到的diffuse值与贴图色彩值相乘，加到上面的结果中去（似乎是为了取代atten * 2）。




效果：

![](https://img-blog.csdn.net/20161014090355548)


使用Lambert光照模型的表面着色器的效果：

![](https://img-blog.csdn.net/20161014090431298)


~~（大家来找茬）~~



