# 写的第一个Shader - 逍遥剑客 - CSDN博客
2007年09月19日 02:47:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2918
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/TestShader0.JPG)
第一个茶壶是可以运动的纹理,用时间的正弦值和余弦值加在纹理坐标上产生偏移实现的,可以考虑用来实现水面的各种效果
第二个茶壶是颜色和纹理的混合,多层纹理的混合应该也是同样的道理,就是把颜色值相乘就可以了
float fSinTime0_X;
float fCosTime0_X;
sampler Texture0;
float4 ps_main(float2 TexCoord  : TEXCOORD0
               ) : COLOR0
{   
   float4 texColor = tex2D( Texture0, TexCoord );
   texColor = texColor * float4( 1.0f, fSinTime0_X, fCosTime0_X, 1.0f );
   return( texColor );
}
