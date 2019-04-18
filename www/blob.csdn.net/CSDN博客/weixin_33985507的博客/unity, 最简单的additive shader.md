# unity, 最简单的additive shader - weixin_33985507的博客 - CSDN博客
2015年04月25日 23:28:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：35

Shader "Custom/myAdditive" {
    Properties {
        _MainTex ("Albedo (RGB)", 2D) = "white" {}
    }
    SubShader {
           Tags { "QUEUE"="Transparent" "IGNOREPROJECTOR"="true" "RenderType"="Transparent" }
        Pass {
ZWrite Off
            Blend SrcAlpha One//use (SrcAlpha,One), not (One,One)
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #pragma target 3.0
            #include "UnityCG.cginc"
            sampler2D _MainTex;
            struct myV2F{
                float4 pos:SV_POSITION;//http://wiki.unity3d.com/index.php?title=Shader_Code
                float2 uv    : TEXCOORD0;
            };
            myV2F vert(appdata_base v)  {
                myV2F v2f;
                v2f.pos=mul (UNITY_MATRIX_MVP, v.vertex);
                v2f.uv=v.texcoord;
                return v2f;
            }
            fixed4 frag(myV2F v2f) : COLOR {
                fixed4 c = tex2D (_MainTex, v2f.uv) ;
                return c;
            }
            ENDCG
        }
    }
}
