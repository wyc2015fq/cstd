# 用可编程渲染管线实现phone光照模型 - 逍遥剑客 - CSDN博客
2008年05月04日 23:09:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：3581
Ambient Lighting = Ca*[Ga + sum(Atti*Spoti*Lai)] 
Where:
|Parameter|Default value|Type|Description|
|----|----|----|----|
|Ca|(0,0,0,0)|D3DCOLORVALUE|Material ambient color|
|Ga|(0,0,0,0)|D3DCOLORVALUE|Global ambient color|
|Atteni|(0,0,0,0)|D3DCOLORVALUE|Light attenuation of the ith light. See Attenuation and Spotlight Factor (Direct3D 9).|
|Spoti|(0,0,0,0)|D3DVECTOR|Spotlight factor of the ith light. See **Attenuation and Spotlight Factor (Direct3D 9)**.|
|sum|N/A|N/A|Sum of the ambient light|
|Lai|(0,0,0,0)|D3DVECTOR|Light ambient color of the ith light|
Diffuse Lighting = sum[Cd*Ld*(N.Ldir)*Atten*Spot]
|Parameter|Default value|Type|Description|
|----|----|----|----|
|sum|N/A|N/A|Summation of each light's diffuse component.|
|Cd|(0,0,0,0)|D3DCOLORVALUE|Diffuse color.|
|Ld|(0,0,0,0)|D3DCOLORVALUE|Light diffuse color.|
|N|N/A|D3DVECTOR|Vertex normal|
|Ldir|N/A|D3DVECTOR|Direction vector from object vertex to the light.|
|Atten|N/A|FLOAT|Light attenuation. See Attenuation and Spotlight Factor (Direct3D 9).|
|Spot|N/A|FLOAT|Spotlight factor. See **Attenuation and Spotlight Factor (Direct3D 9)**.|
Specular Lighting = Cs * sum[Ls * (N • H)P * Atten * Spot]
The following table identifies the variables, their types, and their ranges.
|Parameter|Default value|Type|Description|
|----|----|----|----|
|Cs|(0,0,0,0)|D3DCOLORVALUE|Specular color.|
|sum|N/A|N/A|Summation of each light's specular component.|
|N|N/A|D3DVECTOR|Vertex normal.|
|H|N/A|D3DVECTOR|Half way vector. See the section on the halfway vector.|
|P|0.0|FLOAT|Specular reflection power. Range is 0 to +infinity|
|Ls|(0,0,0,0)|D3DCOLORVALUE|Light specular color.|
|Atten|N/A|FLOAT|Light attenuation value. See Attenuation and Spotlight Factor (Direct3D 9).|
|Spot|N/A|FLOAT|Spotlight factor. See **Attenuation and Spotlight Factor (Direct3D 9)**.|
float4x4 matViewProjection;
float4 Light_Position;
float4 Light_Attenuation;
float4 Light_Color;
float4 Ambient_Color;
float4 vViewPosition;
float alpha;
struct VS_INPUT 
{
float4 Position : POSITION0;
float3 Normal    : NORMAL0;
float2 Texcoord : TEXCOORD0;
};
struct VS_OUTPUT 
{
float4 Position : POSITION0;
float2 Texcoord : TEXCOORD0;
float4 Color     : COLOR0;
};
float4 PointLighting(float3 position, float3 normal
                    , float3 eyedir, float4 ambient
                   , float3 lightpos, float3 lightatt, float4 lightcolor)
{
float3 lightdir = normalize(position - lightpos);
float3 dis = distance(position, lightpos);
float disatt = saturate(1 / ( lightatt.x + lightatt.y * dis + lightatt.z * dis * dis ));
float angleatt = saturate(-lightdir * normal);
float3 reflectdir = normalize(reflect(lightdir, normal));
float specularatt = pow(saturate(reflectdir * eyedir), alpha);
    return ambient + lightcolor * disatt * (angleatt + specularatt);
}
VS_OUTPUT vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;
   Output.Position = mul( Input.Position, matViewProjection );
   Output.Texcoord = Input.Texcoord;
float3 eyedir = normalize(vViewPosition - Input.Position);
   Output.Color = PointLighting(Input.Position, Input.Normal
                       , eyedir, Ambient_Color
                       , Light_Position, Light_Attenuation, Light_Color);
   return( Output );
}
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/Phone_Illumination_Shading.JPG)
