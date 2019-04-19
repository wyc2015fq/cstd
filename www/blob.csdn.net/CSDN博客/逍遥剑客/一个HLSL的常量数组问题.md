# 一个HLSL的常量数组问题 - 逍遥剑客 - CSDN博客
2010年10月02日 13:46:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：3446

在RenderMonkey里写了RNM的demo:
float fUvScale;
sampler2D diffuseMap;
sampler2D normalMap;
sampler2D lightMap1;
sampler2D lightMap2;
sampler2D lightMap3;
const float3 bumpBasis[3] =
{
    {sqrt(2.0 / 3.0), 0.0, 1.0 / sqrt(3.0)},
    {-1.0 / sqrt(6.0), -1.0 / sqrt(2.0), 1.0 / sqrt(3.0)},
    {-1.0 / sqrt(6.0), 1.0 / sqrt(2.0), 1.0 / sqrt(3.0)},
};
struct PS_INPUT 
{
   float2 Texcoord0 : TEXCOORD0;
   float3 Texcoord1 : TEXCOORD1;
};
float4 ps_main( PS_INPUT Input ) : COLOR0
{
   float3 normal = normalize((tex2D(normalMap, Input.Texcoord0).xyz * 2.0f) - 1.0f );
   normal.y = -normal.y;
 
   float3 dp;
   dp.x = saturate(dot(normal, bumpBasis[0]));
   dp.y = saturate(dot(normal, bumpBasis[1]));
   dp.z = saturate(dot(normal, bumpBasis[2]));
   dp *= dp;
   float sum = dot(dp, float3(1.0, 1.0, 1.0));
   dp *= 2.0;
   Input.Texcoord1 *= fUvScale;
   float3 diffuseLighting = dp.x * tex2D(lightMap1, Input.Texcoord1) +
                            dp.y * tex2D(lightMap2, Input.Texcoord1) +
                            dp.z * tex2D(lightMap3, Input.Texcoord1);
   diffuseLighting /= sum;
      
   float4 diffuse = tex2D(diffuseMap, Input.Texcoord0);
   diffuse.xyz = diffuseLighting;
   
   return diffuse;
}
效果在这里面是正确的. 然后转到引擎里发现竟然变成这样了:
![](http://hi.csdn.net/attachment/201010/2/0_1285997866RQ0I.gif)
检查了贴图没问题, 那么只可能是bumpBasis的问题了. 把下面的引用换成float3(...)这种写死的表达式, 果然效果正确了:
![](http://hi.csdn.net/attachment/201010/2/0_12859978088ccS.gif)
要说环境有什么不同, 引擎里是写在.fx文件里的. 难道编译的时候被当成了外部传入的参数? 查了一下HLSL的说明, 发现有个修饰词:
|**static**|Mark a local variable so that it is initialized one time and persists between function calls. If the declaration does not include an initializer, the value is set to zero. A global variable marked *static* is not visible to an application.|
把const float3 bumpBasis[3]改成static const float3 bumpBasis[3], 果然问题没有了!
问题又来了, 为啥在RM里就是好的....而且以前我自己写类似功能时候也没有加static啊-_-
