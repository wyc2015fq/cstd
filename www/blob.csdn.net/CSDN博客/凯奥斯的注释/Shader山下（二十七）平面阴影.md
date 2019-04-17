# Shader山下（二十七）平面阴影 - 凯奥斯的注释 - CSDN博客





2017年12月09日 11:56:31[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：635
所属专栏：[Shader山下](https://blog.csdn.net/column/details/12981.html)









以下摘录自《Unity 3D ShaderLab开发实战详解》，第10章。  

转注：这是一个非常简单且廉价的实时假阴影，比较常用到，据我所知，王者荣耀中就使用了类似的手法。缺点就是比较假，只能投射到平面上，不能应对地面的凹凸，最明显的问题就是会插到墙里去。

转注：需要传给shader接受阴影的地面（平面）的worldToLocalMatrix和localToWorldMatrix。对应的变量分别为_World2Ground和_Ground2World。

```
Shader "Tut/Shadow/PlanarShadow_3" {
//转注：这里对原作里的平行光计算部分和阴影淡出部分做了整合，略去了点光源计算部分。
    Properties{
        _Intensity("atten",range(1,16))=1
    }
    SubShader {
        Pass {
            Tags {"LightMode" = "ForwardBase"}
            Material {Diffuce(1,1,1,1)}
            Lighting On
        }
        Pass {
            Tags {"LightMode" = "ForwardBase"}
            Cull Front
            Blend DstColor SrcColor
            Offset -1,-1 //使阴影在平面之上
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #inlcude "UnityCG.cginc"
            float4x4 _World2Ground;
            float4x4 _Ground2World;
            float _Intensity;
            struct v2f{
                float4 pos:SV_POSITION;
                float atten:TEXCOORD0;
            };
            v2f vert(float4 vertex:POSITION)
            {
                v2f o;
                float3 litDir;
                litDir = WorldSpaceLightDir(vertex);
                litDir = mul(float3x3(_World2Ground), litDir);//把光源方向转换到接受平面空间
                litDir = normalize(litDir);
                float4 vt;
                vt = mul(_Object2World, vertex);
                vt = mul(_World2Ground, vt);//将物体顶点转换到接受平面空间
                vt.xz=vt.xz-(vt.y/litDir.y)*litDir.xz;//用三角形相似计算沿光源方法投射后的xz
                //上面这行代码可拆解为如下的两行代码，这样可能在进行三角形相似计算时更好理解
                //vt.x=vt.x-(vt.y/litDir.y)*litDir.x;
                //vt.z=vt.z-(vt.y/litDir.y)*litDir.z;
                vt.y=0;//使阴影保持在接受平面上
                vt=mul(_Ground2World,vt);//返回到世界坐标空间
                vt=mul(_World2Object,vt);//将结果重新表达为Object Space坐标
                o.pos = mul(UNITY_MATRIX_MVP, vt);//经典的MVP变换，输出到Clip Space
                o.atten=distance(vertex,vt)/_Intensity;//根据前后定点的距离计算衰减
                return o;
            }
            float4 frag(void) : COLOR
            {
                return smoothstep(0,1,i.atten/2);
            }
            ENDCG
        }
    }
}
```

在此Shader中，我们首先使用固定管线对物体做了一个简单的照明。在计算阴影的ForwardBase中，首先使用一个可以叠加阴影的混合模式，然后使用z偏移保证出来的阴影在接受平面之上。_World2Ground和_Ground2World分别是我们自定义的两个进出阴影接受平面矩阵（转注：这里原作解释的不好，按字面意思理解即可）。在具体计算中，首先将光源方向和投影物体的顶点都转换到接受平面的空间（接受平面的物体空间），在它们都处于同一空间后，通过简单的三角形近似算法，来计算投影物体顶点沿光线投影后在接受平面上的新位置。因为这是一个Build In的（Built-in）Unity Plane，所以只计算其xz分量即可，并将y分量设为0，这样投影出来的阴影就出现在接受平面上了。投影计算完成后，返回世界空间，然后到投影物体本身的Object Space，之后的事情就如通常那样，一个经典的MVP变换即可。 

这个方法还有一个显而易见的问题，那就是物体本身是立体的，不在一个平面，因此，这个计算前后的点的距离是包括物体本身厚度的，这个厚度就会表现在阴影上。 

转注：使用Stencil可以解决这个问题~~，但是原作中并没有涉及，所以暂时略过~~。 
算了，以下为原创：
![这里写图片描述](https://img-blog.csdn.net/20171209110618345?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZWNpZGV2aWxpbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如上图所示，Cube和Cylinder貌似没有什么问题，但是右边那个绳结形状的物体的阴影就出现了问题。解决方法，在第二个pass里CGPROGRAM之前，加入以下代码：
```
Stencil{
            Ref 1           //参考值为1，stencilBuffer值默认为0  
            Comp Greater    //stencil比较方式是大于
            Pass replace    //通过的处理是替换，就是拿1替换buffer 的值  
            Fail Keep       //深度检测和模板检测双失败的处理是保持
            ZFail keep      //深度检测失败的处理是保持
        }
```

结果： 
![这里写图片描述](https://img-blog.csdn.net/20171209111046964?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZWNpZGV2aWxpbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

效果略好，但是也并不太理想，那是因为原作并不是把绳结物体当成一整个物体对待，而是当做一个一个的顶点处理，用原顶点和对应的影子顶点计算距离明显有问题。另外有一种做法是传入对象的世界坐标，然后跟世界空间的影子顶点计算距离，但其实也略复杂了，最简单的就是把`o.atten=distance(vertex,vt)/_Intensity;`这一段替换成：`o.atten=length(vt)/_Intensity;`
其实就是计算影子顶点和物体的原点的距离。 

效果如图： 
![这里写图片描述](https://img-blog.csdn.net/20171209112148927?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZWNpZGV2aWxpbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这就需要在制作模型的时候把物体的原点设在y=0的平面上，这样这种影子才能达到较好的效果。 

最后……需要提一句，原作中将物体和影子在一个shader里渲染，我认为这是有问题的，因为没法分别设置Queue，物体的Queue应该是Geometry，而影子的Queue应该是Transparent。所以正确的做法应该是，物体该怎么渲染就怎么渲染，影子做一个GameObject挂在物体上，添加MeshFilter和MeshRenderer使用相同的Mesh，然后MeshRenderer里使用阴影的材质。即上文中的Shader去掉第一个Pass，并修改Tags。Blend方法建议修改为Blend SrcAlpha OneMinusSrcAlpha，最后frag方法建议修改为：
```
float4 frag(v2f i) : COLOR 
        {
            return float4(0,0,0, smoothstep(1, 0, i.atten / 2));
        }
```

综上所述，整合一下代码：

```
Shader "Tut/Shadow/PlanarShadow_Chaos" {
    Properties{
        _Intensity("atten",range(1,16))=1
    }
    SubShader {
        Tags{ "LightMode" = "ForwardBase"
        "Queue" = "Transparent" "IgnoreProjector" = "True" "RenderType" = "Transparent" 
    }
    pass {
        Cull Front
        Blend SrcAlpha OneMinusSrcAlpha
        Offset -1,-1
        Stencil{
            Ref 1           //参考值为1，stencilBuffer值默认为0  
            Comp Greater    //stencil比较方式是大于
            Pass replace    //通过的处理是替换，就是拿1替换buffer 的值  
            Fail Keep       //深度检测和模板检测双失败的处理是保持
            ZFail keep      //深度检测失败的处理是保持
        }
        CGPROGRAM
        #pragma vertex vert 
        #pragma fragment frag
        #include "UnityCG.cginc"
        float4x4 _World2Ground;
        float4x4 _Ground2World;
        float _Intensity;
        struct v2f{
            float4 pos:SV_POSITION;
            float atten:TEXCOORD0;
        };
        v2f vert(float4 vertex: POSITION)
        {
            v2f o;
            float3 litDir;
            litDir=normalize(WorldSpaceLightDir(vertex));  
            litDir=mul(_World2Ground,float4(litDir,0)).xyz;
            float4 vt;
            vt= mul(unity_ObjectToWorld, vertex);
            vt=mul(_World2Ground,vt);
            vt.xz=vt.xz-(vt.y/litDir.y)*litDir.xz;
            vt.y=0;
            vt=mul(_Ground2World,vt);//back to world
            vt=mul(unity_WorldToObject,vt);
            o.pos=UnityObjectToClipPos(vt);
            o.atten=length(vt)/_Intensity;
            return o;
        }
        float4 frag(v2f i) : COLOR 
        {
            return float4(0,0,0, smoothstep(1, 0, i.atten / 2));
        }
        ENDCG 
        }//
   }
}
```






