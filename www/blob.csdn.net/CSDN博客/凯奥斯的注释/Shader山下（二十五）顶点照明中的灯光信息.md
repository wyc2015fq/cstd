# Shader山下（二十五）顶点照明中的灯光信息 - 凯奥斯的注释 - CSDN博客





2017年12月07日 20:01:00[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：260
所属专栏：[Shader山下](https://blog.csdn.net/column/details/12981.html)









以下摘录自[《Unity 3D ShaderLab开发实战详解》](https://book.douban.com/subject/25892263/)，6.4.7-6.4.8。

# 顶点照明中的灯光信息小结

对于LightMode=Vertex的Pass来说，有效存取广元的方法是读取unity_LightPosition[4]和unity_LightColor[4]，这两个数组可以保证在Unity的3个RenderingPath下都有效工作。需要注意的是，unity_LightPosition[4]中的点光源的位置向量和平行光的方向向量都处于CameraSpace（转注：观察空间）中。如果读者想写一个只逐顶点照明的Shader，又不想操作这些恼人的数组，可以直接使用UnityCG.cginc中的ShadeVertexLights函数。 

其次需要牢记一点，Unity不会针对不同的Pass及时清除一些无效数据，你可能会在unity_4LightPos[X, Y, Z]0（转注：unity_4LightPosX0、unity_4LightPosY0和unity_4LightPosZ0的简写）和_WorldSpaceLightPos0中取到某一个物体最后一次执行其他类型的Pass时残留的光源数据，所以，在LightMode为Vertex的Pass内不要使用它们来进行光照计算。

# 一个顶点照明的实际例子

（转注：其实就是把ShadeVertexLights函数的代码稍微改了一下）

```
Shader "Tut/Lighting/VertexLit/Indicator/unity_LightPosition_0" {
    Properties {
        _Color ("Base Color", Color) = (1,1,1,1)
    }
    SubShader {
        Pass {
            Tags {"LightMode"="Vertex"}
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #include "UnityCG.cginc"
            #inlcude "Lighting.cginc"
            uniform float4 _Color;
            struct vertOut {
                float4 pos:SV_POSITION;
                float4 color:COLOR;
            }
            vertOut vert(appdata_base v)
            {
            //Unity提供的光源数据unity_LightPosition是相对于Camera Space的
                float3 viewpos = mul(UNITY_MATRIX_MV, v.vertex).xyz;
                float3 viewN = mul((float3x3)UNITY_MATRIX_IT_MV, v.normal);
                float3 lightColor = UNITY_LIGHTMODEL_AMBIENT.xyz;

                float3 toLight = unity_LightPosition[0].xyz - viewpos.xyz * unity_LightPosition[0].w;
                float lengthSq = dot(toLight, toLight);
                float atten = 4.0 / (1.0 + lengthSq * unity_LightAtten[0].z);
                float diff = max(0, dot(viewN, normalize(toLight)));
                lightColor += unity_LightColor[0].rgb * (diff * atten);
                vertOut o;
                o.pos = mul(UNITY_MATRIX_MVP, v.vertex);
                o.color = float4(lightColor, 1) * _Color;
                return o;
            }
            float4 frag(vertOut i):COLOR
            {
                return i.color;
            }
            ENDCG
        }
    }
}
```



