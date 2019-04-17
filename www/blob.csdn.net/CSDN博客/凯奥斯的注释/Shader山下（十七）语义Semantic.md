# Shader山下（十七）语义Semantic - 凯奥斯的注释 - CSDN博客





2016年10月18日 14:34:15[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：2729
所属专栏：[Shader山下](https://blog.csdn.net/column/details/12981.html)









# 当在编写Cg或者HLSL着色器程序的时候，我们需要使用语义（Semantic）来指明输入输出变量的“意图”。

例如下面的这段Shader代码：



```cpp
Shader "Custom/VF" {
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
			};

			v2f vert(a2v v) {
				v2f o;
				
				//Transform the vertex to projection space
				o.pos = mul(UNITY_MATRIX_MVP, v.vertex);
				//Get the UV coordinates
				o.uv = TRANSFORM_TEX (v.texcoord, _MainTex); 

				return o;
			}

			float4 frag(v2f i) : COLOR {
				fixed4 texColor = tex2D(_MainTex, i.uv);

				return texColor;
			}
			ENDCG
		}
	} 
	FallBack "Diffuse"
}
```


其中a2v结构里变量后面用冒号隔开的就是语义。


a2v作为vert函数的输入，它里面所标识的语义用来告诉Unity我们需要什么样的数据。

此外，我们看到frag方法名的后面也添加了COLOR语义，这是告诉Unity，这个方法输出一个色彩值，然后Unity就可以把这个色彩值写入GPU。

你可能会注意到vert的输出frag的输入v2f结构里的变量也使用了语义。大部分语义是你可以随便使用的，为了方便vert和frag之间传递数据，但是有一个例外，就是:



```cpp
float4 pos : POSITION;
```

这个必须声明为float4类型，并添加POSITION（或SV_Position，二者区别稍后会讲到）。因为GPU需要知道顶点转换到裁剪空间（参考[Shader山下（十六）坐标空间与转换矩阵](http://blog.csdn.net/ecidevilin/article/details/52837877)）的坐标，而这个坐标便是由vertex
 shader提供的。





Direct3D 10定义了一种新的语义类型，叫做系统数值语义（system-value semantics），它们都是以SV_打头。例如POSITION对应SV_POSITION，COLOR对应SV_Target，DEPTH对应SV_Depth（深度）。虽然对于开发者来讲，它们之间并没有什么明显的区别，但是平台之间的兼容性，却给我们带来了烦恼，例如XBox one就只支持SV语义，而Direct3D
 9就不支持SV语义（所以还是windows自己家打起来了）。所以开发的时候还需要注意。




此外还有几个特殊的语义：



#### VPOS：屏幕像素坐标，用于片段着色器，需要添加#pragma target 3.0编译指令。

例如：


```cpp
Shader "Unlit/Screen Position"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "white" {}
    }
    SubShader
    {
        Pass
        {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #pragma target 3.0

            // note: no SV_POSITION in this struct
            struct v2f {
                float2 uv : TEXCOORD0;
            };

            v2f vert (
                float4 vertex : POSITION, // vertex position input
                float2 uv : TEXCOORD0, // texture coordinate input
                out float4 outpos : SV_POSITION // clip space position output
                )
            {
                v2f o;
                o.uv = uv;
                outpos = UnityObjectToClipPos(vertex);
                return o;
            }

            sampler2D _MainTex;

            fixed4 frag (v2f i, UNITY_VPOS_TYPE screenPos : VPOS) : SV_Target
            {
                // screenPos.xy will contain pixel integer coordinates.
                // use them to implement a checkerboard pattern that skips rendering
                // 4x4 blocks of pixels

                // checker value will be negative for 4x4 blocks of pixels
                // in a checkerboard pattern
                screenPos.xy = floor(screenPos.xy * 0.25) * 0.5;
                float checker = -frac(screenPos.r + screenPos.g);

                // clip HLSL instruction stops rendering a pixel if value is negative
                clip(checker);

                // for pixels that were kept, read the texture and output it
                fixed4 c = tex2D (_MainTex, i.uv);
                return c;
            }
            ENDCG
        }
    }
}
```

因为对于大多数平台，VPOS语义修饰的屏幕坐标变量类型是float4，但是对于Direct3D9确实float2，所以要使用UNITY_VPOS_TYPE这个宏来作为screenPos的类型。





#### VFACE：被渲染的面是否朝向摄像机，用于片段着色器，需要添加#pragma target 3.0编译指令。

例如：


```cpp
Shader "Unlit/Face Orientation"
{
    Properties
    {
        _ColorFront ("Front Color", Color) = (1,0.7,0.7,1)
        _ColorBack ("Back Color", Color) = (0.7,1,0.7,1)
    }
    SubShader
    {
        Pass
        {
            Cull Off // turn off backface culling

            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #pragma target 3.0

            float4 vert (float4 vertex : POSITION) : SV_POSITION
            {
                return UnityObjectToClipPos(vertex);
            }

            fixed4 _ColorFront;
            fixed4 _ColorBack;

            fixed4 frag (fixed facing : VFACE) : SV_Target
            {
                // VFACE input positive for frontbaces,
                // negative for backfaces. Output one
                // of the two colors depending on that.
                return facing > 0 ? _ColorFront : _ColorBack;
            }
            ENDCG
        }
    }
}
```




#### SV_VertexID：这是一个无符号的整形变量，表示顶点的ID，用于顶点着色器，需要添加#pragma target 3.5编译指令。
例如：


```cpp
Shader "Unlit/VertexID"
{
    SubShader
    {
        Pass
        {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #pragma target 3.5

            struct v2f {
                fixed4 color : TEXCOORD0;
                float4 pos : SV_POSITION;
            };

            v2f vert (
                float4 vertex : POSITION, // vertex position input
                uint vid : SV_VertexID // vertex ID, needs to be uint
                )
            {
                v2f o;
                o.pos = UnityObjectToClipPos(vertex);
                // output funky colors based on vertex ID
                float f = (float)vid;
                o.color = half4(sin(f/10),sin(f/100),sin(f/1000),0) * 0.5 + 0.5;
                return o;
            }

            fixed4 frag (v2f i) : SV_Target
            {
                return i.color;
            }
            ENDCG
        }
    }
}
```










